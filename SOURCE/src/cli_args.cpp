#include "cli_args.hpp"

#include <cstring>
#include <iomanip>
#include <stdexcept>

bool parse_bool(const char *s, const char *arg_name) { return true; }

int parse_int(const char *s, const char *arg_name) {
    int value;
    auto [ptr, ec] = std::from_chars(s, s + std::strlen(s), value);
    if (ec != std::errc() || *ptr != '\0') {
        throw InvalidValueError(std::string(arg_name));
    }

    return value;
}

auto parse_int_range(int min, int max) {
    return [min, max](const char *s, const char *arg_name) {
        int value = parse_int(s, arg_name);
        if (value < min || value > max) {
            throw InvalidValueError(std::string(arg_name) + " (expected " + std::to_string(min) +
                                    "..=" + std::to_string(max) + ")");
        }

        return value;
    };
}

std::string parse_string(const char *s, const char *arg_name) {
    if (!s || *s == '\0') {
        throw MissingValueError(std::string(arg_name));
    }

    return s;
}

using ParserFn = void (ArgParser::*)(ArgList &);
std::unordered_map<std::string, ParserFn> ArgParser::flag_parsers = {
#define X(flag, field, type, parser, default_val, help) {flag, &ArgParser::parse_##field},
    ARG_TABLE(X)
#undef X
};

#define X(flag, field, type, parser, default_val, help)                                                                \
    void ArgParser::parse_##field(ArgList &list) {                                                                     \
        if constexpr (std::is_same_v<type, bool>) {                                                                    \
            list.field = true;                                                                                         \
            this->arg_i--;                                                                                             \
        } else if (this->arg_i >= this->n_args) {                                                                      \
            this->print_help();                                                                                        \
            throw std::runtime_error("expected a string value for " #field);                                           \
        }                                                                                                              \
        list.field = parser(this->values[this->arg_i], #field);                                                        \
        list.provided_flags.insert(flag);                                                                              \
    }
ARG_TABLE(X)
#undef X

void ArgParser::print_help() const {
    this->out << "usage: " << this->prog_name << " [flags]\nwhere:\n";
#define X(flag, field, type, parser, default_val, help)                                                                \
    this->out << "  " << std::left << std::setw(20) << flag << std::setw(80) << help << "(default: " << default_val    \
              << ")\n";
    ARG_TABLE(X)
#undef X
}

ArgParser::ArgParser(size_t n_args_, char **values_, std::ostream &out_)
    : n_args(n_args_), values(values_), prog_name(values_[0]), out(out_) {}

// assumes that CLI args are <flag_1> <value_1> <flag_2> <value_2> ...
ArgList ArgParser::parse_all() {
    // skip prog_name
    this->arg_i++;
    ArgList list{};

    while (this->arg_i < this->n_args) {
        const char *flag = this->values[this->arg_i];

        auto iter = flag_parsers.find(flag);
        if (iter == flag_parsers.end()) {
            throw UnknownFlagError(std::string(flag));
        }
        const auto &parser = iter->second;
        // move to the flag's value and parse it
        this->arg_i++;
        (this->*parser)(list);
        // move to the next flag
        this->arg_i++;
    }

    return list;
}
