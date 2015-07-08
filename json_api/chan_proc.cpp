#include "chan_proc.hpp"
#include "kyukon/task.hpp"
#include "kyukon/kon.hpp"
#include "../filesystem.hpp"
#include <ctime>

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

    for (const auto &thread_id : thread_ids) {

        std::cout << status(board_url, thread_id) << std::endl;
        auto thread_url = mk_thread_url(board, thread_id);
        auto thread_task = task(1, 
            thread_url, 
            t.get_url(), task::STRING, nullptr);

        k.grab(&thread_task);
        if (thread_task.get_status_code() == 404)
            continue;

        auto posts = p->parse_posts(thread_task.get_data(), chan_post(board, thread_id));

        if (posts.empty()) {
            std::cout << "No posts!" << std::endl;
            std::cout << "Url = " << thread_url << std::endl;
            std::cout << "Data = " << thread_task.get_data() << std::endl;
        }

        for (const auto &post : posts) {
            proc_post(post, thread_task);
        }
    }
    
    return true;
}

bool chan_proc::proc_post(
    const chan_post &post, 
    const task &thread_task)
{
    if (db.post_exists(post))
        return true;

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
    if (filename.find("m") != std::string::npos)
        return true;

    std::cout << status(cp.get_board(), cp.get_thread_id(), filename) 
        << std::endl;
    auto file_url = mk_file_url(cp.get_board(), filename);
    auto file_task = task(1, 
        file_url, 
        thread_task.get_url(), task::FILE, nullptr);

    fs::create_path(cp.get_board());
    file_task.set_filepath(cp.get_board());
    
    k.grab(&file_task);
    if (file_task.get_status_code() == 404)
        return false;

    db.store_file(cp.get_id(), cp.get_board(), filename);
    return true;
}

chan_proc::chan_proc(kon k, chan_db db, chan_parser *p) : k(k), db(db), p(p) {}

chan_proc::chan_proc() 
    : chan_proc(kon("127.0.0.1:9050", true), chan_db("chan"), nullptr) {}

std::string chan_proc::get_base_url(const char *site_name)
{
    if (base_url.empty()) {
        try {
            base_url = std::string(getenv(site_name));
        } catch (const std::logic_error &e) {
            std::cout << "\n~~~ Sorry, please make sure that the environmental "
                "variable \n   $" << site_name << " is set correctly. ~~~\n" 
                << std::endl;
            throw;
        }
    }

    return base_url;
}
