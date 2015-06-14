#include "chan_proc.hpp"
#include "kyukon/task.hpp"
#include "kyukon/kon.hpp"

std::string chan_proc::domain = "4cdn.org";
//std::string chan_proc::domain = "fullchan4jtta4sx.onion";
std::string chan_proc::text_sub = "a";
std::string chan_proc::file_sub = "i";


std::string chan_proc::mk_file_url(const std::string b, const std::string filename)
{
    return "http://" + file_sub + "." + domain + "/" + b + "/" + filename;
}

std::string chan_proc::mk_thread_url(const std::string b, 
    const std::string thread_id)
{
    return "http://" + text_sub + "." + domain + "/" + b 
        + "/thread/" + thread_id + ".json";
}

std::string chan_proc::mk_board_url(const std::string b)
{
    return "http://" + text_sub + "." + domain + "/" + b + "/threads.json";
}

#include <iostream>
bool chan_proc::proc_board(const std::string board)
{
    auto board_url = chan_proc::mk_board_url(board);
    std::cout << "Processing " << board_url << std::endl;

	auto t = task(1, 
        board_url, 
        "http://" + chan_proc::domain, task::STRING, nullptr);

    k.grab(&t);

    auto thread_ids = p.parse_threads(t.get_data());
    if (thread_ids.empty()) {
        std::cout << "No thread ids!" << std::endl;
        //Bad thigns
        //we should at least have the OP right?
        //Check for 403/404
    }

    for (const auto &thread_id : thread_ids) {

        std::cout << "Processing thread " << thread_id << std::endl;
        auto thread_url = mk_thread_url(board, thread_id);
        auto thread_task = task(1, 
            thread_url, 
            t.get_url(), task::STRING, nullptr);

        k.grab(&thread_task);
        auto posts = p.parse_posts(thread_task.get_data(), chan_post(board, thread_id));

        if (posts.empty()) {
            std::cout << "No posts!" << std::endl;
            std::cout << "Url = " << thread_url << std::endl;
            std::cout << "Data = " << thread_task.get_data() << std::endl;
        }

        for (const auto &post : posts) {
            
            if (db.post_exists(post))
                continue;

            for (const auto &filename : post.get_filenames()) {

                // Not every post has an attached file,
                if (filename.empty())
                    continue;

                std::cout << "Porcessing file " << filename << std::endl;
                auto file_url = chan_proc::mk_file_url(board, filename);
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

chan_proc::chan_proc() : k("127.0.0.1:9050", true), db("chan") {}
