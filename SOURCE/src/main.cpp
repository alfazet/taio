#include "algorithms.hpp"
#include "cli_args.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <print>
#include <sstream>
#include <vector>

#define BRUTE 0
#define MITM 1
#define PSEUDOPOLY 2

AlgorithmImpl select_algorithm(int id) {
    switch (id) {
    case BRUTE:
        return &algorithms::brute_force;
    case MITM:
        std::println(stderr, "not implemented yet");
        break;
    case PSEUDOPOLY:
        return &algorithms::pseudo_polynomial;
    }

    // fallback
    return &algorithms::brute_force;
}

std::optional<std::vector<int>> read_testcase(std::istream &input) {
    std::vector<int> testcase{};
    std::string line{};

    if (!std::getline(input, line))
        return {};

    int x{};
    std::istringstream iss(line);
    while (iss >> x)
        testcase.push_back(x);
    if (testcase.empty()) {
        throw std::runtime_error("invalid testcase");
    }

    return testcase;
}

void main_(const ArgList &args) {
    std::optional<std::ifstream> file{};
    if (args.was_provided("-i")) {
        file.emplace(args.input_file);
        if (!*file)
            throw std::runtime_error("could not open the input file");
    }
    std::istream &input = (file ? file.value() : std::cin);

    AlgorithmImpl impl = select_algorithm(args.algorithm);
    auto f = [impl](const std::vector<int> &s) { return impl(s); };

    int tc = 1;
    std::optional<std::vector<int>> s{};
    while ((s = read_testcase(input)) && s.has_value()) {
        Solution solution = f(s.value());
        std::println("testcase {}\n{}\n", tc, solution);
        tc++;
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
    } catch (const std::runtime_error &e) {
        std::println(stderr, "runtime error: {}", e.what());
        return 1;
    }

    return 0;
}
