#include "bigint.hpp"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cerr << "Uzycie: " << argv[0]
                  << "<plik> <z>\nPlik powinien zawierac dane w formacie: <+|-> "
                     "<liczba1> <liczba2>\nObecnosc <z> okresla czy program ma zapisac"
                     " wynik operacji\n";
        return 1;
    }

    bool save_result = argc > 2;

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku " << argv[1] << "\n";
        return 2;
    }

    char op;
    std::string liczba1, liczba2;

    if (!(file >> op >> liczba1 >> liczba2)) {
        std::cerr << "Blad odczytu danych z pliku\n";
        return 3;
    }

    BigInt a(liczba1);
    BigInt b(liczba2);
    BigInt result;
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;

    begin = std::chrono::steady_clock().now();

    switch (op) {
    case '+':
        result = a + b;
        break;
    case '-':
        result = a - b;
        break;
    default:
        std::cerr << "Nieprawidlowa operacja: '" << op << "'\n";
        return 1;
    }

    end = std::chrono::steady_clock().now();

    std::string outfile_name = std::string(argv[1]) + ".out";
    std::ofstream outfile(outfile_name);
    if (!outfile.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku wyjsciowego " << outfile_name << "\n";
        return 4;
    } else {
        outfile << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
        if (save_result) {
            outfile << " " << result;
        }
        outfile << "\n";
    }

    return 0;
}
