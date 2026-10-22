#ifndef TAIO_INPUT_HPP
#define TAIO_INPUT_HPP

#include <iostream>
#include <vector>

namespace input {

int read_int(std::istream &input);

std::vector<int> read_testcase(std::istream &input);

}; // namespace input

#endif // TAIO_INPUT_HPP
