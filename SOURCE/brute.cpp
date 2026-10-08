#include <iostream>
#include <vector>
#include "utils.h"

using namespace std;

vector<vector<int>> solve1(const vector<int>& a) {
    return {};
}

vector<vector<int>> solve2(const vector<int>& a) {
    return {};
}

int main() {
    vector<vector<int>> examples = read_examples(cin);
    for (size_t i = 0; i < examples.size(); i++) {
        cout << "Example " << i + 1 << ": ";
        print_example(examples[i]);
        cout << "  solve1: ";
        print_solution(solve1(examples[i]));
        cout << "  solve2: ";
        print_solution(solve2(examples[i]));
    }
    return 0;
}
