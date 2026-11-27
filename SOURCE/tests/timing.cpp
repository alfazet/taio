/*
 * This test requires input file. It's format is as follows:
 * - First line describes <seed> <retries count> <timeout limit [s]>
 * - Following lines describe single test:
 *   <algo> <param> <from> <to> <step> <fixed n> <fixed l> <fixed k>
 *   where <param> is 'n', 'l' or 'k' depending on which parameter is to be tested and
 *   <from>, <to> and <step> describe change of chosen parameter
 */

#include "bigint.hpp"
#include "algorithms.hpp"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

std::mt19937 rng;

std::vector<BigInt> generate(int n, int l) {
    std::string s;
    std::vector<BigInt> set;

    for (int i = 0; i < n; i++) {
        s.clear();
        s += char('1' + rng() % 9);
        for (int j = 1; j < l; j++) {
            s += char('0' + rng() % 10);
        }
        set.push_back(BigInt(s));
    }

    return set;
}

double run_once(int algo, int k, int n, int l) {
    auto set = generate(n, l);
    auto before = std::chrono::steady_clock::now();
    if (algo == 0) {
        algorithms::brute_force(set, k);
    } else {
        algorithms::karmarkar_karp(set, k);
    }
    auto after = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(after - before).count();
    return static_cast<double>(diff);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Uzycie: " << argv[0] << " <wejscie.txt> <wyniki.csv>\n";
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
    double limit;
    if (!(in >> seed >> retries >> limit)) {
        std::cerr << "Niepoprawny naglowek pliku wejsciowego\n";
        return 3;
    }
    rng.seed(seed);

    out << "algo,param,value,n,l,k,time_us\n";

    int algo, from, to, step, n, l, k;
    char param, prev_param = 0;
    while (in >> algo >> param >> from >> to >> step >> n >> l >> k) {
        if (prev_param != param) {
            std::cerr << "===> algo: " << algo << "; param: " << param << '\n';
        }

        prev_param = param;
        for (int v = from; v <= to; v += step) {
            switch (param) {
                case 'n': n = v; break;
                case 'k': k = v; break;
                case 'l': l = v; break;
            }

            double sum = 0;
            int r = 0;
            bool timeout = false;
            for (r = 0; r < retries; r++) {
                double t = run_once(algo, k, n, l);
                sum += t;
                if (t > limit * 1e6) {
					v = to + 1;
                    break;
                }
            }

            double mean = sum / r;

            out << algo << ',' << param << ',' << v << ',' << n << ',' << l << ','
				<< k << ',' << mean << '\n';
            std::cerr << "algo=" << algo << " n=" << n << " l=" << l << " k=" << k
				<< ": " << mean << " us\n";
        }
    }

    return 0;
}

