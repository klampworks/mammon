#pragma once
#include <string>
#include <vector>
#include "fourchan_parser_json.hpp"
#include "kyukon/kon.hpp"
#include "chan_db.hpp"

class chan_proc {

    static std::string domain;
    static std::string text_sub;
    static std::string file_sub;

    fourchan_parser_json p;
    kon k;
    chan_db db;

    public:
        static std::string mk_file_url(const std::string b, const std::string filename);
        static std::string mk_thread_url(const std::string b, 
            const std::string thread_id);

        static std::string mk_board_url(const std::string b);

        bool proc_board(const std::string board);
        chan_proc();
};
