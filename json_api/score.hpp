#pragma once
#include <functional>

class thread;

namespace score {

    extern std::function<int(const char*, const char*)> ext_score;
    void score(thread &t);
}
