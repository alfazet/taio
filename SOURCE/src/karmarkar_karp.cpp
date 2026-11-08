#include "algorithms.hpp"

#include <algorithm>
#include <numeric>
#include <queue>

namespace algorithms {

struct KKTuple {
    std::vector<std::vector<BigInt>> subsets{};
    std::vector<BigInt> sums{};
    BigInt spread{};

    void update_spread() {
        auto mm = std::minmax_element(this->sums.begin(), this->sums.end());
        this->spread = *mm.second - *mm.first;
    }

    bool operator<(const KKTuple &o) const { return this->spread < o.spread; }
};

Solution karmarkar_karp(const std::vector<BigInt> &s, int n_subsets) {
    Solution best(n_subsets);
    if (s.empty())
        return best;

    std::vector<KKTuple> temp;
    temp.reserve(s.size());
    for (const BigInt &x : s) {
        KKTuple t;
        t.subsets.assign(n_subsets, std::vector<BigInt>());
        t.sums.assign(n_subsets, BigInt{0});
        t.subsets[0].push_back(x);
        t.sums[0] = x;
        t.update_spread();
        temp.push_back(std::move(t));
    }
    std::priority_queue<KKTuple> heap(std::less<KKTuple>{}, std::move(temp));

    // moves the vectors out of the pq.top() tuple instead of copying them
    // and keeps `spread` intact so that the corrent item gets popped
    auto pop_and_move = [&heap]() {
        KKTuple &top = const_cast<KKTuple &>(heap.top());
        KKTuple t;
        t.subsets = std::move(top.subsets);
        t.sums = std::move(top.sums);
        t.spread = top.spread;
        heap.pop();

        return t;
    };

    std::vector<int> idx_a(n_subsets), idx_b(n_subsets);
    while (heap.size() > 1) {
        KKTuple a = pop_and_move();
        std::iota(idx_a.begin(), idx_a.end(), 0);
        std::sort(idx_a.begin(), idx_a.end(), [&](int x, int y) { return a.sums[x] < a.sums[y]; });

        KKTuple b = pop_and_move();
        std::iota(idx_b.begin(), idx_b.end(), 0);
        std::sort(idx_b.begin(), idx_b.end(), [&](int x, int y) { return b.sums[x] > b.sums[y]; });

        KKTuple c;
        c.subsets.resize(n_subsets);
        c.sums.assign(n_subsets, BigInt{0});

        for (int i = 0; i < n_subsets; i++) {
            std::vector<BigInt> &dest_subset = c.subsets[i];
            std::vector<BigInt> &subset_a = a.subsets[idx_a[i]];
            std::vector<BigInt> &subset_b = b.subsets[idx_b[i]];

            dest_subset = std::move(subset_a);
            if (dest_subset.size() < subset_b.size())
                dest_subset.swap(subset_b);
            // dest_subset is the larger of the two, so whenever any element
            // is moved to dest_subset, it's moved from a subset of size S to a subset of size at least 2 * S
            //
            // since there are n elements and at most log n moves can happen to a given element
            // the total number of moves is n log n
            dest_subset.insert(dest_subset.end(), std::make_move_iterator(subset_b.begin()),
                               std::make_move_iterator(subset_b.end()));
            c.sums[i] = a.sums[idx_a[i]] + b.sums[idx_b[i]];
        }

        c.update_spread();
        heap.push(std::move(c));
    }
    KKTuple last = pop_and_move();
    best.subsets = std::move(last.subsets);
    best.abs_sum_diff = last.spread;

    return best;
}

}; // namespace algorithms
