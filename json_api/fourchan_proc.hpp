#pragma once
#include <string>

class fourchan_proc {

    static std::string domain;
    static std::string text_sub;
    static std::string file_sub;

    public:
        static std::string mk_file_url(const std::string b, const std::string filename);
        static std::string mk_thread_url(const std::string b, 
            const std::string thread_id);

        static std::string mk_board_url(const std::string b);

};
