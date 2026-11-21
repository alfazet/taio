/*
 * This test requires input file. It's format is as follows:
 * - First line describes <seed> <retries count>
 * - Following lines describe single test:
 *   <from> <to> <step> <fixed n> <fixed l>
 *   where <from>, <to> and <step> describe change of chosen parameter and
 *   <fixed n> and <fixed l> are values of 'n' and 'l' parameters respectively
 */

#include "algorithms.hpp"
#include "bigint.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
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
		auto value = BigInt(s);
        set.push_back(value);
    }

    return set;
}

double measure(int retries, int n, int l, int k) {
	
	double ratio = 0;

	for (int i = 0; i < retries; ++i) {
		auto set = generate(n, l);
		auto solution_approx = algorithms::karmarkar_karp(set, k);
		auto solution_exact = algorithms::brute(set, k);
		ratio += solution_approx.abs_sum_diff.double_div(solution_exact.abs_sum_diff);
	}

	return ratio / static_cast<double>(retries);
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
    if (!(in >> seed >> retries)) {
        std::cerr << "Niepoprawny naglowek pliku wejsciowego\n";
        return 3;
    }
    rng.seed(seed);

	out << "k,n,l,error\n";
	
	int from, to, step, n, l;
	while (in >> from >> to >> step >> n >> l) {
        for (int k = from; k <= to; k += step) {
            auto error = measure(retries, n, l, k);

            out << k << ',' << n << ',' << l << ',' << error << std::endl;
            std::cerr << "k=" << k << " n=" << n << " l=" << l << ": " << error << "\n";
        }
    }
}

