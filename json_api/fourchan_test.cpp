#include "fourchan_proc.hpp"

#include "eightchan_proc.hpp"

#include <functional>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <map>

#include <chibi/eval.h>
#include "score.hpp"
#include "ext.hpp"

void process(std::unique_ptr<chan_proc> pc, std::vector<std::string> boards)
{
    for (const auto &b: boards)
        pc->proc_board(b);
}

#define bind(type) \
    [](std::vector<std::string> bs) { \
        process(std::unique_ptr<chan_proc>(new type()), bs);},

int main(int argc, char **argv)
{
    std::map<std::string, std::function<void(std::vector<std::string>)>> 
        opts = {
            {"4chan", bind(fourchan_proc)},
            {"8chan", bind(eightchan_proc)}};

    if (argc < 3) {
        std::cout << "Usage " << argv[0] << " <domain> board1 board2 ...\n";
        std::cout << "\tWhere <domain> may be one of the following:\n";
        for (const auto &o: opts)
            std::cout << "\t\t" << o.first << "\n";

        std::cout << std::endl;
        return 1;
    }

    sexp ctx = sexp_make_eval_context(NULL, NULL, NULL, 0, 0);
    sexp_load_standard_env(ctx, NULL, SEXP_SEVEN);

    sexp_gc_var1(score);
    sexp_gc_preserve1(ctx, score);

    bool ret = ext::load_file(ctx, "mammon.ss");

    score = sexp_eval_string(ctx, "score", -1, NULL);
    if (sexp_procedurep(score)) {
        score::ext_score = [&ctx](const char *t) {

        return ext::call_fun_str(ctx, "score", t);
        };
    }

    opts.at(argv[1])(std::vector<std::string>(argv+2, argv+argc));

    sexp_gc_release1(ctx);
    sexp_destroy_context(ctx);
    return 0;
}
