#include "utils.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int get_sum_diff(const vector<vector<int>> &groups) {
    if (groups.empty())
        return 0;
    int max_sum = 0;
    int min_sum = 0;
    for (size_t g = 0; g < groups.size(); g++) {
        int sum = 0;
        for (size_t i = 0; i < groups[g].size(); i++) {
            sum += groups[g][i];
        }
        if (g == 0) {
            max_sum = sum;
            min_sum = sum;
        } else {
            max_sum = max(max_sum, sum);
            min_sum = min(min_sum, sum);
        }
    }
    return max_sum - min_sum;
}

vector<vector<int>> read_examples(istream &in) {
    vector<vector<int>> examples;
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<int> example;
        int x;
        while (iss >> x)
            example.push_back(x);
        if (!example.empty())
            examples.push_back(example);
    }
    return examples;
}

void print_example(const vector<int> &example) {
    cout << "[";
    for (size_t i = 0; i < example.size(); i++) {
        if (i)
            cout << ", ";
        cout << example[i];
    }
    cout << "]\n";
}

void print_solution(const vector<vector<int>> &solution) {
    cout << "{";
    for (size_t i = 0; i < solution.size(); i++) {
        if (i)
            cout << ", ";
        cout << "[";
        for (size_t j = 0; j < solution[i].size(); j++) {
            if (j)
                cout << ", ";
            cout << solution[i][j];
        }
        cout << "]";
    }
    cout << "} with max-min = ";
    cout << get_sum_diff(solution) << "\n";
}
