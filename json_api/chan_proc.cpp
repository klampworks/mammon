#include "chan_proc.hpp"
#include "kyukon/task.hpp"
#include "kyukon/kon.hpp"
#include "../filesystem.hpp"
#include <ctime>
#include <algorithm>
#include "hash.hpp"
#include <fstream>
#include "score.hpp"

std::string chan_proc::now()
{
    time_t t = time(NULL);
    struct tm *tm_info;
    tm_info = localtime(&t);

    char buf[20];
    strftime(buf, 20, "%F %T", tm_info);
    return std::string(buf);
}

std::string chan_proc::status(const std::string &board)
{
    return now() + " Processing board=[" + board + "]";
}

std::string chan_proc::status(const std::string &board, 
    const std::string &thread_id)
{
    return status(board) + " thread=[" + thread_id + "]";
}

std::string chan_proc::status(const std::string &board, 
    const std::string &thread_id,
    const std::string &filename)
{
    return status(board, thread_id) + " file=[" + filename + "]";
}

#include <iostream>
bool chan_proc::proc_board(const std::string board)
{
    fs::create_path(board);
    auto board_url = mk_board_url(board);
    std::cout << status(board_url) << std::endl;

	auto t = task(1, 
        board_url, 
        "http://google.com", task::STRING, nullptr);
        //"http://" + chan_proc::domain, task::STRING, nullptr);

    k.grab(&t);

    auto thread_ids = p->parse_threads(t.get_data());
    if (thread_ids.empty()) {
        std::cout << "Fuck." << std::endl;
        // Either they are down or refusing us service.
    }

    for (auto &thread_id : thread_ids) {

        std::cout << status(board_url, thread_id) << std::endl;
        auto thread_url = mk_thread_url(board, thread_id);
        auto thread_task = task(1, 
            thread_url, 
            t.get_url(), task::STRING, nullptr);

        k.grab(&thread_task);
        if (thread_task.get_status_code() == 404)
            continue;

        {
        const std::string path = board + "/" + thread_id;
        fs::create_path(path);

        std::ofstream ofs;
        ofs.open(path + "/" + thread_id + ".json");
        ofs << thread_task.get_data();
        ofs.close();
        }

        thread t(
            p->parse_posts(thread_task.get_data(), chan_post(board, thread_id)),
            thread_task);

        score::score(t);

        if (t.score > 0) {
            proc_thread(t);
        } else {
            std::cout << "Skipping thread=[" << thread_id
                << "] becuase score=[" << t.score << "] is too low."
                << std::endl;
        }
    }

    return true;
}

void chan_proc::proc_thread(const thread &thread)
{
        const auto &posts = thread.posts;
        const auto &thread_task = thread.thread_task;
        std::cout << thread.score << std::endl;

        //TODO Means the thread fell off page 10 or was deleted.
        // In the latter case it may have had interesting content...
        //if (posts.empty())

        for (auto rit = posts.crbegin(); rit != posts.crend(); ++rit) {

            if (!proc_post(*rit, thread_task)) {
                // We have already seen this post. Since we are going backwards
                // through the thread it means we have seen all of the earlier 
                // ones too.
                break;
            }
        };
}

bool chan_proc::proc_post(
    const chan_post &post, 
    const task &thread_task)
{
    if (db.post_exists(post))
        return false;

    for (const auto &filename : post.get_filenames())
        proc_file(post, filename, thread_task);

    db.store_post(post);

    return true;
}

bool chan_proc::proc_file(
    const chan_post &cp, 
    const std::string filename,
    const task &thread_task)
{
    // Not every post has an attached file,
    if (filename.empty() || db.file_exists(cp.get_board(), filename))
        return true;

    //Quick hack to avoid large files (webM, Mp4 etc.)
    //if (filename.find("m") != std::string::npos)
    //    return true;

    std::cout << status(cp.get_board(), cp.get_thread_id(), filename) 
        << std::endl;

    auto file_url = mk_file_url(cp.get_board(), filename);
    auto file_task = task(1, 
        file_url, 
        thread_task.get_url(), task::FILE, nullptr);

    {
    const std::string new_path = cp.get_board() + "/" + cp.get_thread_id();
    file_task.set_filepath(new_path);
    }
    
    k.grab(&file_task);
    if (file_task.get_status_code() == 404)
        return false;

    std::string fullname = file_task.get_filepath() + "/" + filename.c_str();
    std::string md5 = hash::md5(fullname.c_str());
    const auto pair = fs::split_ext(filename);
    std::string new_fullname = file_task.get_filepath() + "/" + 
        md5 + "." + pair.second;

    if (fs::file_exists(new_fullname) || db.file_hash_exists(md5)) {
        remove(fullname.c_str());
    } else {
        rename(fullname.c_str(), new_fullname.c_str());
        db.store_file(cp.get_id(), cp.get_board(), filename, md5);
    }

    return true;
}

chan_proc::chan_proc(kon k, chan_db db, chan_parser *p) : k(k), db(db), p(p) {}

chan_proc::chan_proc() 
    : chan_proc(kon("127.0.0.1:9050", true), chan_db("chan"), nullptr) {}
