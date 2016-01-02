#include "filesystem.hpp"
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <climits>

namespace fs {

    std::pair<std::string, std::string> split_ext(const std::string fn)
    {
        const auto pos = fn.rfind('.');
        if (pos == std::string::npos) {
            return {fn, ""};
        }

        return {fn.substr(0, pos), fn.substr(pos+1)};
    }

    std::string get_ext(const std::string fn)
    {
        const auto pos = fn.rfind('.');
        if (pos == std::string::npos) {
            return "";
        }

        return fn.substr(pos);
    }

    bool move(const std::string &from, const std::string &to)
    {
        return rename(from.c_str(), to.c_str()) == 0;
    }

    void append_slash(std::string &path)
    {
        /* Make sure the path ends with a forward slash. */
        if (path.size() > 1 && path.rfind("/") + 1 != path.size())
            path += "/";
    }

    bool file_exists(const std::string &path)
    {
        struct stat stat_buf;
        return stat(path.c_str(), &stat_buf) == 0;
    }

    bool create_path(const std::string &path)
    {
        struct stat stat_buf;
        int res = stat(path.c_str(), &stat_buf);

        /* No errors from stat. */
        if (!res) {
            
            /* What we want exists and is a directory. */
            if (S_ISDIR(stat_buf.st_mode)) {
                goto good;
            }

            std::cout << "Error file exists but is not a directory." 
                << std::endl; 
            goto bad;

        } else if (errno == ENOENT) {

            /* Path does not exist, good. */
            if (!mkdir(path.c_str(), 0777)) {
                goto good;
            } else {
                std::cout << "Error creating directory: " << 
                errno << std::endl;
                goto bad;
            }
        }

        std::cout << "Stat error " << errno << std::endl;
        goto bad;

        good:
            return true;

        bad:
            return false;
    }

    void unique_fn(std::string &fn)
    {
        struct stat sb;
        std::string original(fn);
        int post = 0; 

        for (;;) {

            int res = stat(fn.c_str(), &sb);


            if (res == -1) {
                if (errno == ENOENT) { 

                    /* File does not exist, good. */
                    return;
                }

                std::cout << "Stat failed for file <" << fn 
                    << "> because " << strerror(errno) << 
                    " HTML was probably not dumped." 
                    << std::endl;
                return;
            }

            /* Prevent integer overflow from leading to an infinit loop. */
            if (post < INT_MAX)
                ++post;
            else
                return;
                /* If you have 4 billion files with the same name, 
                 * overwriting the last one is the least of your 
                 * problems.*/

            fn = original + "_" + std::to_string(post);
        }
    }
}
