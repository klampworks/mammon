#include "fourchan_proc.hpp"
#include "kyukon/task.hpp"
#include "kyukon/kon.hpp"

std::string fourchan_proc::domain = "4cdn.org";
std::string fourchan_proc::text_sub = "a";
std::string fourchan_proc::file_sub = "i";


std::string fourchan_proc::mk_file_url(const std::string b, const std::string filename)
{
    return "http://" + file_sub + "." + domain + "/" + b + "/" + filename;
}

std::string fourchan_proc::mk_thread_url(const std::string b, 
    const std::string thread_id)
{
    return "http://" + text_sub + "." + domain + "/" + b 
        + "/thread/" + thread_id + ".json";
}

std::string fourchan_proc::mk_board_url(const std::string b)
{
    return "http://" + text_sub + "." + domain + "/" + b + "/threads.json";
}

bool fourchan_proc::proc_board(const std::string board)
{
    auto board_url = fourchan_proc::mk_board_url(board);

	auto t = task(1, 
        board_url, 
        "http://" + fourchan_proc::domain, task::STRING, nullptr);

    k.grab(&t);

    auto thread_ids = p.parse_threads(t.get_data());
    if (thread_ids.empty()) {
        //Bad thigns
        //we should at least have the OP right?
        //Check for 403/404
    }

    for (const auto &thread_id : thread_ids) {

        auto thread_url = mk_thread_url(board, thread_id);
        auto thread_task = task(1, 
            thread_url, 
            t.get_url(), task::STRING, nullptr);

        k.grab(&thread_task);
        auto posts = p.parse_posts(thread_task.get_data(), chan_post(board, thread_id));

        for (const auto &post : posts) {
            
            if (db.post_exists(post))
                continue;

            for (const auto &filename : post.get_filenames()) {
                auto file_url = fourchan_proc::mk_file_url(board, filename);
                auto file_task = task(1, 
                    file_url, 
                    thread_task.get_url(), task::FILE, nullptr);
                k.grab(&file_task);
                //CHeck result
            }

            db.store_post(post);
        }

    }
    
    return true;
}

fourchan_proc::fourchan_proc() : k("127.0.0.1:9050", true), db("fourchan") {}
