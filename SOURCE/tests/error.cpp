/*
 * This test requires input file. It's format is as follows:
 * - First line describes <seed> <retries count>
 * - Following lines describe single test:
 *   <param> <from> <to> <step> <fixed n> <fixed l> <fixed k>
 *   where <param> is 'n', 'l' or 'k' depending on which parameter is to be tested and
 *   <from>, <to> and <step> describe change of chosen parameter
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

std::tuple<BigInt, std::vector<BigInt>> generate(int n, int l) {
    std::string s;
    std::vector<BigInt> set;
	BigInt sum = BigInt(0);

    for (int i = 0; i < n; i++) {
        s.clear();
        s += char('1' + rng() % 9);
        for (int j = 1; j < l; j++) {
            s += char('0' + rng() % 10);
        }
		auto value = BigInt(s);
        set.push_back(value);
		sum += value;
    }

    return {sum, set};
}

double measure(int retries, int n, int l, int k) {
	
	double ratio = 0;

	for (int i = 0; i < retries; ++i) {
		auto generated = generate(n, l);
		auto total = std::get<0>(generated);
		auto set = std::get<1>(generated);
		auto solution = algorithms::karmarkar_karp(set, k);
		auto maxdiff = solution.abs_sum_diff;
		ratio += maxdiff.double_div(total);
	}

	return ratio / static_cast<double>(retries);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Użycie: " << argv[0] << " <wejście.txt> <wyniki.csv>\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);
    if (!in || !out) {
        std::cerr << "Nie udało się otworzyć plików\n";
        return 2;
    }

    int seed;
    int retries;
    if (!(in >> seed >> retries)) {
        std::cerr << "Niepoprawny nagłówek pliku wejściowego\n";
        return 3;
    }
    rng.seed(seed);

	out << "param,n,l,k,error\n";
	
	int from, to, step, n, l, k;
	char param, prev_param = 0;
	while (in >> param >> from >> to >> step >> n >> l >> k) {
        for (int v = from; v <= to; v += step) {
			switch (param) {
                case 'n': n = v; break;
                case 'k': k = v; break;
                case 'l': l = v; break;
            }

            auto error = measure(retries, n, l, k);

            out << param << ',' << n << ',' << l << ',' << k << ','
                << error << std::endl;
            std::cerr << "n=" << n << " l=" << l << " k=" << k << ": " << error << "\n";
        }
    }
}

