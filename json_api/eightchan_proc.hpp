#pragma once
#include "chan_proc.hpp"
#include <string>

class eightchan_proc : public chan_proc
{
    public:
        virtual std::string mk_file_url(const std::string b, 
            const std::string filename);
        virtual std::string mk_thread_url(const std::string b, 
            const std::string thread_id);
        virtual std::string mk_board_url(const std::string b);

        eightchan_proc(std::string &&b_url);
    private:
        const std::string& mk_base_url() const;
        const std::string base_url;
};
