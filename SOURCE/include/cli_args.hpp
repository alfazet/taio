#ifndef TAIO_CLI_ARGS_HPP
#define TAIO_CLI_ARGS_HPP

#include <limits>
#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

constexpr const char *DEFAULT_INPUT_FILE = "";
constexpr int DEFAULT_ALGORITHM = 0;
constexpr int DEFAULT_N_SUBSETS = 2;

#define ARG_TABLE(X)                                                                                                   \
    X("--help", help, bool, parse_bool, false, "pokazuje opis dostępnych opcji")                                       \
    X("-i", input_file, std::string, parse_string, DEFAULT_INPUT_FILE,                                                 \
      "plik z danymi wejściowymi, program czyta z wejścia standardowego jeżeli nie podano pliku")                      \
    X("-a", algorithm, int, parse_int_range(0, 3), DEFAULT_ALGORITHM,                                                  \
      "numer wybranego algorytmu (0/1/2/3 = brute-force/meet-in-the-middle/pseudo-wielomianowy/karmarkar-karp)")       \
    X("-k", n_subsets, int, parse_int_range(2, std::numeric_limits<int>::max()), DEFAULT_N_SUBSETS,                    \
      "na ile podzbiorów podzielić dany zbiór")

struct ArgList {
    std::string dataset_path;

    std::unordered_set<std::string> provided_flags;

    bool was_provided(const char *flag) const { return provided_flags.contains(flag); }

#define X(flag, field, type, parser, default_val, help) type field = default_val;
    ARG_TABLE(X)
#undef X
};

class ArgParser {
  public:
    size_t n_args;
    char **values;
    char *prog_name;
    size_t arg_i = 0;
    std::ostream &out;

    ArgParser(size_t n_args_, char **values_, std::ostream &out_);

    ArgList parse_all();

    void print_help() const;

  private:
    static std::unordered_map<std::string, void (ArgParser::*)(ArgList &)> flag_parsers;

#define X(flag, field, type, parser, default_val, help) void parse_##field(ArgList &list);
    ARG_TABLE(X)
#undef X
};

class ArgParserError : public std::runtime_error {
  public:
    using std::runtime_error::runtime_error;
};

class UnknownFlagError : public ArgParserError {
  public:
    explicit UnknownFlagError(std::string flag) : ArgParserError("nieznana opcja: " + flag), flag(std::move(flag)) {}

    std::string flag;
};

class MissingValueError : public ArgParserError {
  public:
    explicit MissingValueError(std::string flag) : ArgParserError("brak wartości dla " + flag) {}
};

class InvalidValueError : public ArgParserError {
  public:
    InvalidValueError(std::string flag) : ArgParserError("nieprawidłowa wartość dla " + flag) {}
};

#endif // TAIO_CLI_ARGS_HPP
