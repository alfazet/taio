#ifndef TAIO_INPUT_HPP
#define TAIO_INPUT_HPP

#include "bigint.hpp"

#include <iostream>
#include <vector>

namespace input {

int read_int(std::istream &input);

std::vector<BigInt> read_testcase(std::istream &input);

}; // namespace input

#endif // TAIO_INPUT_HPP
