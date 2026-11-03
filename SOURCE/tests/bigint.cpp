#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "bigint.hpp"

int main(int argc, char **argv) {

	if (argc < 2) {
		std::cerr << "Użycie: " << argv[0] <<
			"<plik> <z>\nPlik powinien zawierać dane w formacie: <+|-> "
			"<liczba1> <liczba2>\nObecność <z> określa czy program ma zapisać"
			" wynik operacji\n";
		return 1;
	}

	bool save_result = argc > 2;

    std::ifstream file(argv[1]);

    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku " << argv[1] << "\n";
        return 2;
    }

    char op;
    std::string liczba1, liczba2;

    if (!(file >> op >> liczba1 >> liczba2)) {
        std::cerr << "Błąd odczytu danych z pliku\n";
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
            std::cerr << "Nieprawidłowa operacja: '" << op << "'\n";
            return 1;
    }

	end = std::chrono::steady_clock().now();

	std::string outfile_name = std::string(argv[1]) + ".out";
	std::ofstream outfile(outfile_name);
	if (!outfile.is_open()) {
		std::cerr << "Nie można otworzyć pliku wyjściowego " << outfile_name << "\n";
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

