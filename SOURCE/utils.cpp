#include "utils.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

vector<vector<int>> read_examples(istream& in) {
    vector<vector<int>> examples;
    string line;
    while (getline(in, line)) {
        istringstream iss(line);
        vector<int> example;
        int x;
        while (iss >> x) example.push_back(x);
        if (!example.empty()) examples.push_back(example);
    }
    return examples;
}

void print_example(const vector<int>& example) {
    cout << "[";
    for (size_t i = 0; i < example.size(); i++) {
        if (i) cout << ", ";
        cout << example[i];
    }
    cout << "]\n";
}

void print_solution(const vector<vector<int>>& solution) {
    cout << "{";
    for (size_t i = 0; i < solution.size(); i++) {
        if (i) cout << ", ";
        cout << "[";
        for (size_t j = 0; j < solution[i].size(); j++) {
            if (j) cout << ", ";
            cout << solution[i][j];
        }
        cout << "]";
    }
    cout << "}\n";
}
