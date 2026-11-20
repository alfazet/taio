/*
 * This test requires input file. It's format is as follows:
 * - First line describes <main program binary path> <retries count> <timeout limit [s]>
 * - Following lines describe single test:
 *   <algo> <param> <from> <to> <step> <fixed n> <fixed l> <fixed k>
 *   where param is 'n', 'l' or 'k' depending on which parameter is to be tested and
 *   from, to and step describe change of chosen parameter
 */

#include "bigint.hpp"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

std::mt19937 rng(2137);

void generate(const char *path, int n, int l) {
    std::ofstream f(path);
    f << "1\n";
    for (int i = 0; i < n; i++) {
        f << char('1' + rng() % 9);
        for (int j = 1; j < l; j++) f << char('0' + rng() % 10);
        f << ' ';
    }
    f << '\n';
}

double run_once(const std::string &bin, int algo, int k, int n, int l) {
    generate(".timing_in.txt", n, l);
    std::string cmd = bin + " -i .timing_in.txt -a " + std::to_string(algo) + " -k " + std::to_string(k) + " -t > .timing_out.txt";
    if (std::system(cmd.c_str()) != 0) {
        std::cerr << "Błąd: polecenie nie powiodło się: " << cmd << "\n";
        std::exit(1);
    }
    double t = 0;
    std::ifstream f(".timing_out.txt");
    f >> t;
    return t;
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

    std::string bin;
    int retries;
    double limit;
    in >> bin >> retries >> limit;
#ifdef _WIN32
    std::replace(bin.begin(), bin.end(), '/', '\\');
#endif

    out << "algo,param,value,n,l,k,time_us\n";

    int algo, from, to, step, n, l, k;
    char prev_param = 0, param;
    while (in >> algo >> param >> from >> to >> step >> n >> l >> k) {
		if (prev_param != param)
			std::cerr << "===> algo: "<< algo << "; param: " << param << std::endl;
		prev_param = param;
        for (int v = from; v <= to; v += step) {
			switch (param) {
				case 'n': n = v; break;
				case 'k': k = v; break;
				case 'l': l = v; break;
			}

            double sum = 0;
			int r = 0;
            for (r = 0; r < retries; r++) {
				double t = run_once(bin, algo, k, n, l);
				sum += t;
				if (t > limit * 1e6) {
					v = to + 1;
					break;
				}
			}
            double mean = sum / r;

            out << algo << ',' << param << ',' << v << ',' << n << ',' << l << ','
                << k << ',' << mean << std::endl;
            std::cerr << "algo=" << algo << " n=" << n << " l=" << l << " k=" << k
                      << ": " << mean << " us\n";
        }
    }

    std::remove(".timing_in.txt");
    std::remove(".timing_out.txt");
	std::remove(".timing_in_output.txt");
    return 0;
}
