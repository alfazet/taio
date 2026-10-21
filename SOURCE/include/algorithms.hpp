#ifndef TAIO_ALGORITHMS_HPP
#define TAIO_ALGORITHMS_HPP

#include <format>
#include <vector>

#define MAX_N_BRUTE 32
#define MAX_N_MITM 64
#define MAX_NK_PSEUDOPOLY 0x7fffffff
#define MAX_NM_PSEUDOPOLY 0x7fffffff

#define INF 0x7fffffff

struct Solution {
    std::vector<std::vector<int>> subsets;
    int abs_sum_diff;

    explicit Solution(int n_subsets) : subsets{std::vector<std::vector<int>>(n_subsets)}, abs_sum_diff{INF} {}

    inline void reset() {
        for (int i = 0; i < this->subsets.size(); i++)
            this->subsets[i].clear();
        this->abs_sum_diff = INF;
    }
};

template <> struct std::formatter<Solution> : std::formatter<std::string> {
    auto format(const Solution &s, std::format_context &ctx) const {
        std::string repr =
            std::format("largest absolute difference of sums over all subsets:\n{}\nsubsets:", s.abs_sum_diff);
        for (const auto &subset : s.subsets) {
            repr += std::format("\n{}", subset);
        }

        return std::formatter<std::string>::format(repr, ctx);
    }
};

namespace algorithms {

Solution brute_force(const std::vector<int> &s, int n_subsets);

// TODO
Solution meet_in_the_middle(const std::vector<int> &s, int n_subsets);

Solution pseudo_polynomial(const std::vector<int> &s, int n_subsets);

Solution karmarkar_karp(const std::vector<int> &s, int n_subsets);

}; // namespace algorithms

using AlgorithmImpl = Solution (*)(const std::vector<int> &, int);

#endif // TAIO_ALGORITHMS_HPP
