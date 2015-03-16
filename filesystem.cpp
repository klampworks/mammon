#include "filesystem.hpp"
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

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
}
