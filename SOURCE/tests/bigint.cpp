#include <fstream>
#include <iomanip>
#include <print>
#include "bigint.hpp"

int main(int argc, char **argv) {
	
	if (argc < 4) {
		std::cerr << "Użycie: " << argv[0] << " <+|-> <liczba1> <liczba2>\n";
		return 1;
	}

	char op = argv[1][0];
	BigInt a(argv[2]);
	BigInt b(argv[3]);
	BigInt result;

	switch(op) {
		case '+':
			result = a + b;
			break;
		case '-':
			result = a - b;
			break;
		default:
			std::cerr << "Nieprawidłowa operacja: '" << op << "'\n";
			return -1;
			break;
	}

	std::cout << result;

	return 0;
}
