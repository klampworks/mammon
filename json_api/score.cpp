#include "score.hpp"
#include "chan_post.hpp"
#include <functional>

namespace score {

    std::function<int(const char*, const char*)> ext_score = nullptr;

    void score(thread &t)
    {
        if (!ext_score) {
            t.score = 1;
        } else {
            for (const auto &p: t.posts) {
                t.score += ext_score(p.get_board().c_str(),
                    p.get_com().c_str());
            }
        }
    }
}
