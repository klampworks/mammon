#include "hash.hpp"
#include <openssl/md5.h>
#include <cstring>
#include <cstdio>
#include <string>

namespace hash {

    std::string md5(const char *filename)
    {
        char hash[33];
        md5(filename, hash);
        return std::string(hash);
    }

    void md5(const char *filename, char *hash)
    {
        FILE *in = fopen(filename, "rb");
        if (!in)
            return;

        MD5_CTX ctx;
        MD5_Init(&ctx);

        unsigned char data[1024];
        int bytes;
        while ((bytes = fread(data, 1, 1024, in)) != 0) {
            MD5_Update(&ctx, data, bytes);
        }

        unsigned char c[MD5_DIGEST_LENGTH];
        MD5_Final(c, &ctx);

        char tmp[2];
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            sprintf(tmp, "%02x", c[i]);
            memcpy(hash+(i*2), tmp, 2);
        }

        hash[MD5_DIGEST_LENGTH*2] = '\0';
        fclose(in);
    }
};
