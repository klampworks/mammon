#pragma once
#include "chan_proc.hpp"
#include <string>

class fourchan_proc : public chan_proc
{
    public:
        virtual std::string mk_file_url(const std::string b, 
            const std::string filename);
        virtual std::string mk_thread_url(const std::string b, 
            const std::string thread_id);
        virtual std::string mk_board_url(const std::string b);

        fourchan_proc();

    private:
        std::string mk_filebase_url();
        std::string mk_textbase_url();
};
