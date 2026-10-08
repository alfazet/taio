#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

void add_element(const vector<int> &a, size_t index, vector<vector<int>> &groups, vector<vector<int>> &res, int &best) {
    if (index == a.size()) {
        int curr = get_sum_diff(groups);
        if (curr < best) {
            best = curr;
            res = groups;
        }
        return;
    }
    for (size_t g = 0; g < groups.size(); g++) {
        groups[g].push_back(a[index]);
        add_element(a, index + 1, groups, res, best);
        groups[g].pop_back();
    }
}

vector<vector<int>> solve(const vector<int> &a, int k) {
    vector<vector<int>> res;
    vector<vector<int>> groups(k > 0 ? k : 0);
    int sum = 0;
    for (size_t i = 0; i < a.size(); i++)
        sum += a[i];
    int best = sum;
    add_element(a, 0, groups, res, best);
    return res;
}

int main() {
    vector<vector<int>> examples = read_examples(cin);
    for (size_t i = 0; i < examples.size(); i++) {
        cout << "Example " << i + 1 << ": ";
        print_example(examples[i]);
        for (int k = 2; k < 5; k++) {
            cout << " solve k = " << k << ": ";
            print_solution(solve(examples[i], k));
        }
    }
    return 0;
}
