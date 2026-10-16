#ifndef TAIO_CLI_ARGS_HPP
#define TAIO_CLI_ARGS_HPP

#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

constexpr const char *DEFAULT_INPUT_FILE = "";
constexpr int DEFAULT_ALGORITHM = 0;

#define ARG_TABLE(X)                                                                                                   \
    X("--help", help, bool, parse_bool, false, "show help?")                                                           \
    X("-i", input_file, std::string, parse_string, DEFAULT_INPUT_FILE,                                                 \
      "input file with testcases (reads from stdin if not provided)")                                                  \
    X("-a", algorithm, int, parse_int_range(0, 2), DEFAULT_ALGORITHM,                                                  \
      "id of the algorithm to run (0/1/2 = brute/meet-in-the-middle/pseudo-poly)")

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
    explicit UnknownFlagError(std::string flag) : ArgParserError("unknown flag: " + flag), flag(std::move(flag)) {}

    std::string flag;
};

class MissingValueError : public ArgParserError {
  public:
    explicit MissingValueError(std::string flag) : ArgParserError("missing value for " + flag) {}
};

class InvalidValueError : public ArgParserError {
  public:
    InvalidValueError(std::string flag) : ArgParserError("invalid value for " + flag) {}
};

#endif // TAIO_CLI_ARGS_HPP
