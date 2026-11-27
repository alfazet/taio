#include "bigint.hpp"

#include <cstdlib>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

static std::mt19937_64 rng;

static long long rand_number(int digits) {
    long long lo = 1;
    for (int i = 1; i < digits; i++)
        lo *= 10;
    long long hi = lo * 10 - 1;
    std::uniform_int_distribution<long long> dist(lo, hi);
    return dist(rng);
}

static bool single_test(char op, int digits) {
    long long num1 = rand_number(digits);
    long long num2 = rand_number(digits);

    if (op == '-' && num2 > num1)
        std::swap(num1, num2);

    BigInt a(std::to_string(num1));
    BigInt b(std::to_string(num2));
    BigInt result;
    long long expected;

    if (op == '+') {
        result = a + b;
        expected = num1 + num2;
    } else {
        result = a - b;
        expected = num1 - num2;
    }

    std::ostringstream oss;
    oss << result;
    std::string actual = oss.str();
    std::string expected_str = std::to_string(expected);

    bool passed = (actual == expected_str);
    std::cout << "Status: [" << (passed ? "ZDANE" : "NIE ZDANE") << "] | " << num1 << " " << op << " " << num2 << " = "
              << actual << " | Oczekiwany wynik: " << expected_str << "\n";

    return passed;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Uzycie: " << argv[0] << " <seed>\n";
        return 1;
    }

    char *end = nullptr;
    unsigned long long seed = std::strtoull(argv[1], &end, 10);
    if (*argv[1] == '\0' || *end != '\0') {
        std::cerr << "Niepoprawny seed: " << argv[1] << "\n";
        return 2;
    }
    rng.seed(seed);

    int total = 0;
    int failed = 0;

    // 18 digits fit in long long
    for (int digits = 1; digits <= 18; digits++) {
        for (int i = 0; i < 20; i++) {
            for (char op : {'+', '-'}) {
                total++;
                if (!single_test(op, digits))
                    failed++;
            }
        }
    }

    std::cout << "\nZdane: " << (total - failed) << "/" << total << "\n";

    return failed == 0 ? 0 : 1;
}
