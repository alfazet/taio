#include "algorithms.hpp"

#include <cmath>
#include <numeric>

namespace algorithms {

Solution brute_force(const std::vector<int> &s, int n_subsets) {
    if (n_subsets != 2)
        throw std::runtime_error("działa tylko dla podziału na 2 podzbiory");

    int n = s.size();
    int sum_total = std::accumulate(s.begin(), s.end(), 0);
    int max_bitmask = (1 << n) - 1;
    Solution best(n_subsets);

    for (int bitmask = 0; bitmask <= max_bitmask; bitmask++) {
        int sum_taken = 0;
        for (int bit = 0; bit <= n - 1; bit++) {
            if ((bitmask & (1 << bit)) == 0)
                sum_taken += s[bit];
        }
        int sum_rem = sum_total - sum_taken;

        int abs_sum_diff = std::abs(sum_taken - sum_rem);
        if (abs_sum_diff < best.abs_sum_diff) {
            best.reset();
            for (int bit = 0; bit <= n - 1; bit++) {
                int which = ((bitmask & (1 << bit)) == 0 ? 0 : 1);
                best.subsets[which].push_back(s[bit]);
            }
            best.abs_sum_diff = abs_sum_diff;
        }
    }

    return best;
}

}; // namespace algorithms
