#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

std::vector<std::vector<int>> read_examples(std::istream &in);
void print_example(const std::vector<int> &example);
void print_examples(const std::vector<std::vector<int>> &examples);
void print_solution(const std::vector<std::vector<int>> &solution);
int get_sum_diff(const std::vector<std::vector<int>> &groups);

#endif
