#include "algorithms.hpp"
#include "cli_args.hpp"
#include "input.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <print>
#include <vector>

#define BRUTE 0
#define MITM 1
#define PSEUDOPOLY 2
#define KARMARKAR_KARP 3

AlgorithmImpl select_algorithm(int id) {
    switch (id) {
    case BRUTE:
        return &algorithms::brute_force;
    case MITM:
        std::println(stderr, "not implemented yet");
        break;
    case PSEUDOPOLY:
        return &algorithms::pseudo_polynomial;
    case KARMARKAR_KARP:
        return &algorithms::karmarkar_karp;
    }

    // fallback
    return &algorithms::brute_force;
}

void main_(const ArgList &args) {
    std::optional<std::ifstream> file{};
    if (args.was_provided("-i")) {
        file.emplace(args.input_file);
        if (!*file)
            throw std::runtime_error("nie udało się otworzyć pliku");
    }
    std::istream &input = (file ? file.value() : std::cin);

    AlgorithmImpl impl = select_algorithm(args.algorithm);
    auto f = [impl](const std::vector<int> &s, int n_subsets) { return impl(s, n_subsets); };

    int n_testcases = input::read_int(input);
    int n_subsets = args.n_subsets;
    std::vector<int> s{};
    for (int tc = 1; tc <= n_testcases; tc++) {
        s = input::read_testcase(input);
        Solution solution = f(s, n_subsets);
        std::println("test {}\n{}\n", tc, solution);
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
        std::println(stderr, "błąd parsera opcji: {}", e.what());
        arg_parser.print_help();
        return 1;
    } catch (const std::runtime_error &e) {
        std::println(stderr, "błąd wykonania: {}", e.what());
        return 1;
    }

    return 0;
}
