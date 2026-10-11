#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

// k = 2 only: subset-sum DP, O(n * sum/2) time and memory. Non-negative integers.
vector<vector<int>> solve(const vector<int> &a) {
    int n = (int)a.size();
    long long K = 0;
    for (int x : a) K += x;

    int half = (int)(K / 2);
    // P[j][i] = true if some subset of the first j numbers sums to exactly i
    vector<vector<bool>> P(n + 1, vector<bool>(half + 1, false));

    P[0][0] = true;
    for (int j = 0; j <= n; j++) P[j][0] = true;
    for (int j = 1; j <= n; j++) {
        int x = a[j - 1];
        for (int i = 0; i <= half; i++)
            P[j][i] = P[j - 1][i] || (i - x >= 0 && P[j - 1][i - x]);
    }

    int s = half;
    while (!P[n][s]) s--;
    vector<vector<int>> res(2);
    for (int j = n; j >= 1; j--) {
        if (P[j - 1][s]) {
            res[1].push_back(a[j - 1]);
        } else {
            res[0].push_back(a[j - 1]);
            s -= a[j - 1];
        }
    }
    return res;
}

int main() {
    vector<vector<int>> examples = read_examples(cin);
    for (size_t i = 0; i < examples.size(); i++) {
        cout << "Example " << i + 1 << ": ";
        print_example(examples[i]);
        for (int k = 2; k < 5; k++) {
            cout << " solve k = " << k << ": ";
            print_solution(solve(examples[i]));
        }
    }
    return 0;
}
