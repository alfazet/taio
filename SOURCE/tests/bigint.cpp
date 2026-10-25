#include <iostream>
#include <fstream>
#include <iomanip>
#include <print>
#include "bigint.hpp"

int main(int argc, char **argv) {

	if (argc < 2) {
		std::cerr << "Użycie: " << argv[0] <<
			"<plik>\nPlik powinien zawierać dane w formacie: <+|-> <liczba1> <liczba2>\n";
		return 1;
	}

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

    std::cout << result;

    return 0;
}

