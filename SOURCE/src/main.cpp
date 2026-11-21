#include "algorithms.hpp"
#include "cli_args.hpp"
#include "input.hpp"
#include "output.hpp"

#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <print>
#include <vector>

#define BRUTE 0
#define KARMARKAR_KARP 1

AlgorithmImpl select_algorithm(int id) {
    switch (id) {
    case BRUTE:
        return &algorithms::brute_force;
    case KARMARKAR_KARP:
        return &algorithms::karmarkar_karp;
    }

    throw std::runtime_error("wybrano nieprawidlowy algorytm (musi byc 0 lub 1)");
}

void main_(const ArgList &args) {
    if (std::strcmp(args.input_file.c_str(), UNDEFINED_INPUT_FILE) == 0)
        throw std::runtime_error("nie okreslono pliku wejsciowego");
    if (args.algorithm == UNDEFINED_ALGORITHM)
        throw std::runtime_error("nie okreslono algorytmu do wykonania");
    if (args.n_subsets == UNDEFINED_N_SUBSETS)
        throw std::runtime_error("nie okreslono liczby podzbiorow w podziale");

    std::string in_file_path = args.input_file;
    std::ifstream in_file(in_file_path);
    if (!in_file)
        throw std::runtime_error("nie udalo sie otworzyc pliku wejsciowego");
    std::istream &input = in_file;

    std::string out_file_path = output::out_file_path(in_file_path);
    std::ofstream out_file(out_file_path);
    if (!out_file)
        throw std::runtime_error("nie udalo sie otworzyc pliku wyjsciowego");
    std::ostream &output = out_file;

    AlgorithmImpl impl = select_algorithm(args.algorithm);
    auto f = [impl](const std::vector<BigInt> &s, int n_subsets) {
        auto before = std::chrono::steady_clock::now();
        auto res = impl(s, n_subsets);
        auto after = std::chrono::steady_clock::now();
        u64 diff = std::chrono::duration_cast<std::chrono::microseconds>(after - before).count();

        return std::pair<Solution, u64>{res, diff};
    };

    bool print_timer = args.print_timer;
    int n_testcases = input::read_int(input);
    int n_subsets = args.n_subsets;
    output::write_pair(output, n_testcases, n_subsets);

    std::vector<BigInt> s{};
    for (int tc = 1; tc <= n_testcases; tc++) {
        s = input::read_testcase(input);
        auto [solution, micros] = f(s, n_subsets);
        output::write_testcase(output, s);
        output::write_solution(output, solution);

        if (print_timer)
            std::println("{}", micros);
    }
    std::println("zapisano rozwiazania do pliku wyjsciowego {}", out_file_path);
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
        std::println(stderr, "blad parsera opcji: {}", e.what());
        arg_parser.print_help();
        return 1;
    } catch (const std::runtime_error &e) {
        std::println(stderr, "blad wykonania: {}", e.what());
        return 1;
    }

    return 0;
}
