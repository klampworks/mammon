#pragma once
#include <string>
#include <vector>
#include "fourchan_parser_json.hpp"
#include "kyukon/kon.hpp"
#include "chan_db.hpp"

class chan_proc {

    protected:
        kon k;
        chan_db db;
        chan_parser *p;

    public:
        virtual std::string mk_file_url(const std::string b, 
            const std::string filename)=0;
        virtual std::string mk_thread_url(const std::string b, 
            const std::string thread_id)=0;
        virtual std::string mk_board_url(const std::string b)=0;

        bool proc_board(const std::string board);
        chan_proc();
        chan_proc(kon k, chan_db db, chan_parser *p);
        bool proc_file(const chan_post &cp, 
            const std::string filename, const task &thread_task);
        virtual ~chan_proc();
};
