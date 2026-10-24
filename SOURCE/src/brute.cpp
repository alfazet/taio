#include "algorithms.hpp"

#include <numeric>

namespace algorithms {

Solution brute_force(const std::vector<BigInt> &s, int n_subsets) {
    if (n_subsets != 2)
        throw std::runtime_error("działa tylko dla podziału na 2 podzbiory");

    Solution best(n_subsets);
    if (s.empty())
        return best;

    int n = s.size();
    BigInt sum_total = std::accumulate(s.begin(), s.end(), BigInt{});
    u64 max_bitmask = (u64{1} << n) - 1;
    for (u64 bitmask = 0; bitmask <= max_bitmask; bitmask++) {
        BigInt sum_taken{};
        for (int bit = 0; bit <= n - 1; bit++) {
            if ((bitmask & (u64{1} << bit)) == 0)
                sum_taken += s[bit];
        }
        BigInt sum_rem = sum_total - sum_taken;

        BigInt abs_sum_diff = abs_diff(sum_taken, sum_rem);
        if (abs_sum_diff < best.abs_sum_diff) {
            best.reset();
            for (int bit = 0; bit <= n - 1; bit++) {
                int which = ((bitmask & (u64{1} << bit)) == 0 ? 0 : 1);
                best.subsets[which].push_back(s[bit]);
            }
            best.abs_sum_diff = abs_sum_diff;
        }
    }

    return best;
}

}; // namespace algorithms
