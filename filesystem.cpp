#include "filesystem.hpp"
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
#include <climits>

namespace fs {

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
        }

        /* Path does not exist, good. */
        if (errno == ENOENT) {
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
