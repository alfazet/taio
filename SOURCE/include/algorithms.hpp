#ifndef TAIO_ALGORITHMS_HPP
#define TAIO_ALGORITHMS_HPP

#include "bigint.hpp"

#include <format>
#include <vector>

struct Solution {
    std::vector<std::vector<BigInt>> subsets;
    BigInt abs_sum_diff;

    explicit Solution(int n_subsets) : subsets(n_subsets), abs_sum_diff{BigInt::make_inf()} {}

    inline void reset() {
        for (usize i = 0; i < this->subsets.size(); i++)
            this->subsets[i].clear();
        this->abs_sum_diff = BigInt::make_inf();
    }
};

template <> struct std::formatter<Solution> : std::formatter<std::string> {
    auto format(const Solution &s, std::format_context &ctx) const {
        std::string repr = std::format("największy moduł z różnic sum po wszystkich parach podzbiorów:\n{}\npodzbiory:",
                                       s.abs_sum_diff);
        for (const auto &subset : s.subsets)
            repr += std::format("\n{}", subset);

        return std::formatter<std::string>::format(repr, ctx);
    }
};

namespace algorithms {

Solution brute_force(const std::vector<BigInt> &s, int n_subsets);

Solution karmarkar_karp(const std::vector<BigInt> &s, int n_subsets);

}; // namespace algorithms

using AlgorithmImpl = Solution (*)(const std::vector<BigInt> &, int);

#endif // TAIO_ALGORITHMS_HPP
