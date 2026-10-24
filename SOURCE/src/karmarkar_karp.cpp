#include "algorithms.hpp"

#include <numeric>

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

    std::vector<KKTuple> heap;
    heap.reserve(s.size());

    for (const BigInt &x : s) {
        KKTuple t;
        t.subsets.assign(n_subsets, std::vector<BigInt>());
        t.sums.assign(n_subsets, BigInt{0});
        t.subsets[0].push_back(x);
        t.sums[0] = x;
        t.update_spread();
        heap.push_back(std::move(t));
    }
    std::make_heap(heap.begin(), heap.end());

    while (heap.size() > 1) {
        std::pop_heap(heap.begin(), heap.end());
        KKTuple a = std::move(heap.back());
        heap.pop_back();

        std::pop_heap(heap.begin(), heap.end());
        KKTuple b = std::move(heap.back());
        heap.pop_back();

        std::vector<int> ia(n_subsets), ib(n_subsets);
        std::iota(ia.begin(), ia.end(), 0);
        std::iota(ib.begin(), ib.end(), 0);

        std::sort(ia.begin(), ia.end(), [&](int x, int y) { return a.sums[x] < a.sums[y]; });
        std::sort(ib.begin(), ib.end(), [&](int x, int y) { return b.sums[x] > b.sums[y]; });

        KKTuple c;
        c.subsets.assign(n_subsets, std::vector<BigInt>());
        c.sums.assign(n_subsets, BigInt{0});

        for (int j = 0; j < n_subsets; j++) {
            std::vector<BigInt> &dst = c.subsets[j];
            std::vector<BigInt> &sa = a.subsets[ia[j]];
            std::vector<BigInt> &sb = b.subsets[ib[j]];

            dst = std::move(sa);
            dst.insert(dst.end(), sb.begin(), sb.end());
            c.sums[j] = a.sums[ia[j]] + b.sums[ib[j]];
        }

        c.update_spread();
        heap.push_back(std::move(c));
        std::push_heap(heap.begin(), heap.end());
    }
    best.subsets = std::move(heap.front().subsets);
    best.abs_sum_diff = heap.front().spread;

    return best;
}

}; // namespace algorithms
