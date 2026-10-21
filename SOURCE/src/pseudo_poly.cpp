#include "algorithms.hpp"

#include <cmath>
#include <numeric>

namespace algorithms {

Solution pseudo_polynomial(const std::vector<int> &s, int n_subsets) {
    if (n_subsets != 2)
        throw std::runtime_error("działa tylko dla podziału na 2 podzbiory");

    int n = s.size();
    int sum_total = std::accumulate(s.begin(), s.end(), 0);
    int half_total_sum = sum_total / 2;

    // dp[i][j] = does some subset of the first `i` numbers sum to `j`?
    std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(half_total_sum + 1));
    dp[0][0] = true;

    for (int i = 0; i <= n; i++)
        dp[i][0] = true;
    for (int i = 1; i <= n; i++) {
        int prev = s[i - 1];
        for (int new_sum = 0; new_sum <= half_total_sum; new_sum++) {
            int need = new_sum - prev;
            dp[i][new_sum] = dp[i - 1][new_sum] || (need >= 0 && dp[i - 1][need]);
        }
    }

    int achieved_sum = half_total_sum;
    while (!dp[n][achieved_sum])
        achieved_sum--;

    Solution best(n_subsets);
    best.abs_sum_diff = std::abs(achieved_sum - (sum_total - achieved_sum));
    for (int i = n; i >= 1; i--) {
        if (dp[i - 1][achieved_sum])
            best.subsets[0].push_back(s[i - 1]);
        else {
            best.subsets[1].push_back(s[i - 1]);
            achieved_sum -= s[i - 1];
        }
    }

    return best;
}

}; // namespace algorithms
