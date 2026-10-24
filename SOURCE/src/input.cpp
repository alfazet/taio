#include "input.hpp"

#include <sstream>

namespace input {

int read_int(std::istream &input) {
    std::string line{};
    if (!std::getline(input, line))
        throw std::runtime_error("błąd wejścia");

    int n{};
    std::istringstream iss(line);
    iss >> n;

    return n;
}

std::vector<BigInt> read_testcase(std::istream &input) {
    std::vector<BigInt> testcase{};
    std::string line{};
    if (!std::getline(input, line))
        throw std::runtime_error("błąd wejścia");

    BigInt x{};
    std::istringstream iss(line);
    while (iss >> x)
        testcase.push_back(x);
    if (testcase.empty())
        throw std::runtime_error("nieprawidłowy test");

    return testcase;
}

}; // namespace input
