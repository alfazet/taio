#include "output.hpp"

#include <algorithm>

namespace output {

std::string out_file_path(const std::string &in_file_path) {
    std::string out = in_file_path;
    if (out.ends_with(".txt"))
        out.resize(out.size() - 4);
    else
        throw std::runtime_error("oczekiwano pliku w formacie txt");
    out += "_output.txt";

    return out;
}

void write_pair(std::ostream &out, int n, int k) { out << n << " " << k; }

void write_testcase(std::ostream &out, const std::vector<BigInt> &s) {
    out << "\n\n";
    BigInt sum{};
    for (const auto &x : s)
        sum += x;
    out << sum;
    for (const auto &x : s)
        out << " " << x;
}

void write_solution(std::ostream &out, Solution &solution) {
    std::sort(solution.subsets.rbegin(), solution.subsets.rend());
    for (auto &subset : solution.subsets) {
        std::sort(subset.rbegin(), subset.rend());
        BigInt sum{};
        for (const auto &x : subset)
            sum += x;
        out << "\n" << sum;
        for (const auto &x : subset)
            out << " " << x;
    }
    out << "\n" << solution.abs_sum_diff;
}

}; // namespace output
