#include "utils.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

const int TOTAL = 100;

vector<int> divide_sum(int n) {
    vector<int> res;
    while (n > 0) {
        int r = rand() % n + 1;
        res.push_back(r);
        n -= r;
    }
    return res;
}

vector<int> gen_example(int k) {
    vector<int> res;
    if (k <= 0)
        return res;
    int sum = TOTAL / k;
    for (int i = 0; i < k; ++i) {
        vector<int> parts = divide_sum(sum);
        res.insert(res.end(), parts.begin(), parts.end());
    }
    return res;
}

vector<vector<int>> gen_examples(int n, int k) {
    vector<vector<int>> res;
    for (int i = 0; i < n; i++) {
        res.push_back(gen_example(k));
    }
    return res;
}

int main() {
    srand(time(nullptr));
    vector<vector<int>> res = gen_examples(100, 2);
    print_examples(res);
    return 0;
}
