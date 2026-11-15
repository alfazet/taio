#ifndef TAIO_OUTPUT_HPP
#define TAIO_OUTPUT_HPP

#include "algorithms.hpp"

#include <iostream>
#include <vector>

namespace output {

std::string out_file_path(const std::string &in_file_path);

void write_pair(std::ostream &out, int n, int k);

void write_testcase(std::ostream &out, const std::vector<BigInt> &s);

void write_solution(std::ostream &out, Solution &solution);

}; // namespace output

#endif // TAIO_OUTPUT_HPP
