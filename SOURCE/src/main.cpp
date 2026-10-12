#include "algorithms.hpp"
#include "cli_args.hpp"

#include <iostream>
#include <print>

#define BRUTE 0
#define MITM 1
#define PSEUDOPOLY 2

AlgorithmImpl select_algorithm(int id) {
    switch (id) {
    case BRUTE:
        return &algorithms::brute_force;
    case MITM:
        break;
    case PSEUDOPOLY:
        break;
    }
    // TODO: do something about the warning
}

void main_(const ArgList &args) {
    AlgorithmImpl impl = select_algorithm(args.algorithm);
    auto f = [impl](const std::vector<int> &s) { return impl(s); };

    int n_cases = 2; // TODO: read from the file
    for (int tc = 1; tc <= n_cases; tc++) {
        std::vector<int> s{9, 2, 6, 1, 4}; // TODO: read from the file
        Solution solution = f(s);
        std::println("testcase {}\n{}\n", tc, solution);
    }
}

int main(int argc, char **argv) {
    ArgParser arg_parser(argc, argv, std::cout);
    try {
        ArgList args = arg_parser.parse_all();
        if (args.help) {
            arg_parser.print_help();
            return 1;
        }
        main_(args);
    } catch (const ArgParserError &e) {
        std::println(stderr, "argument parser error: {}", e.what());
        arg_parser.print_help();
        return 1;
    }

    return 0;
}
