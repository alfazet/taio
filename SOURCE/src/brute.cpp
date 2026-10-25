#include "algorithms.hpp"

#include <numeric>

namespace algorithms {

void brute_force_descent(const std::vector<BigInt> &s, usize current_number, usize used, std::vector<BigInt> &sums,
                         Solution &current, Solution &best) {
    if (best.abs_sum_diff.is_zero())
        return;

    if (current_number == s.size()) {
        auto [min, max] = std::minmax_element(sums.begin(), sums.end());
        BigInt diff = *max - *min;
        if (diff < best.abs_sum_diff) {
            best.subsets = current.subsets;
            best.abs_sum_diff = std::move(diff);
        }
        return;
    }

    usize limit = std::min(used + 1, sums.size());
    for (usize i = 0; i < limit; i++) {
        current.subsets[i].push_back(s[current_number]);
        sums[i] += s[current_number];
        brute_force_descent(s, current_number + 1, (i == used) ? used + 1 : used, sums, current, best);
        sums[i] -= s[current_number];
        current.subsets[i].pop_back();
    }
}

Solution brute_force(const std::vector<BigInt> &s, int n_subsets) {
    Solution best(n_subsets);
    if (s.empty() || n_subsets <= 0)
        return best;

    Solution current(n_subsets);
    std::vector<BigInt> sums(n_subsets, BigInt{});
    brute_force_descent(s, 0, 0, sums, current, best);

    return best;
}

}; // namespace algorithms
