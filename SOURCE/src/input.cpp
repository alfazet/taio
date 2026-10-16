#include "input.hpp"

#include <sstream>

namespace input {

int read_int(std::istream &input) {
    std::string line{};
    if (!std::getline(input, line))
        throw std::runtime_error("input error");

    int n{};
    std::istringstream iss(line);
    iss >> n;

    return n;
}

std::vector<int> read_testcase(std::istream &input) {
    std::vector<int> testcase{};
    std::string line{};
    if (!std::getline(input, line))
        throw std::runtime_error("input error");

    int x{};
    std::istringstream iss(line);
    while (iss >> x)
        testcase.push_back(x);
    if (testcase.empty())
        throw std::runtime_error("invalid testcase");

    return testcase;
}

}; // namespace input
