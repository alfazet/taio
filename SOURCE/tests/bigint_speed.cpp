/*
 * Measures speed of BigInt addition/subtraction depending on length of numbers.
 *
 * Usage: test_bigint_timing <config.txt> <results.csv>
 *
 * Config file format:
 * - First line describes <seed> <retries count>
 * - Following lines describe single test:
 *   <op> <from> <to> <step>
 *   where <op> is '+' or '-' and <from>, <to> and <step> describe change of
 *   length of numbers (in digits), both ends inclusive
 *
 * Output CSV columns: op,digits,time_us (mean time over retries)
 */

#include "bigint.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

std::mt19937 rng;

std::string generate(int digits) {
    std::string s;
    s.reserve(digits);
    s += char('1' + rng() % 9);
    for (int i = 1; i < digits; i++) {
        s += char('0' + rng() % 10);
    }
    return s;
}

double run_once(char op, int digits) {
    BigInt a(generate(digits));
    BigInt b(generate(digits));

    if (op == '-' && a < b)
        std::swap(a, b);

    BigInt result;
    auto before = std::chrono::steady_clock::now();
    if (op == '+') {
        result = a + b;
    } else {
        result = a - b;
    }
    auto after = std::chrono::steady_clock::now();

    volatile bool sink = result.is_zero();
    (void)sink;

    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(after - before).count();
    return static_cast<double>(diff);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Uzycie: " << argv[0] << " <konfiguracja.txt> <wyniki.csv>\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);
    if (!in || !out) {
        std::cerr << "Nie udalo sie otworzyc plikow\n";
        return 2;
    }

    int seed;
    int retries;
    if (!(in >> seed >> retries) || retries <= 0) {
        std::cerr << "Niepoprawny naglowek pliku konfiguracyjnego\n";
        return 3;
    }
    rng.seed(seed);

    out << "op,digits,time_us\n";

    char op;
    int from, to, step;
    while (in >> op >> from >> to >> step) {
        if ((op != '+' && op != '-') || from <= 0 || step <= 0) {
            std::cerr << "Niepoprawna linia konfiguracji: " << op << " " << from << " " << to << " " << step << "\n";
            return 4;
        }

        for (int d = from; d <= to; d += step) {
            double sum = 0;
            for (int r = 0; r < retries; r++) {
                sum += run_once(op, d);
            }
            double mean = sum / retries;

            out << op << ',' << d << ',' << mean << '\n';
            std::cerr << "op=" << op << " digits=" << d << ": " << mean << " us\n";
        }
    }

    return 0;
}
