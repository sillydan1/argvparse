#ifndef AUTOVERIF_ARGUMENTS_H
#define AUTOVERIF_ARGUMENTS_H
#include <utility>
#include <vector>
#include <map>
#include <getopt.h>
#include <sstream>

enum class argument_requirement {
    NO_ARG = 0,
    REQUIRE_ARG = 1,
    OPTIONAL_ARG = 2
};

struct option_t {
    const char* long_option;
    char short_option;
    argument_requirement needs_argument;
    const char* description;
    explicit operator option() const { return {long_option, (int)needs_argument, nullptr, short_option}; }
};

class argument_t {
    bool m_enabled;
    std::string long_name;
    std::string argval;
    char short_name;
    void as_check() const;
public:
    argument_t();
    argument_t(std::string  _long_name, char _short_name, std::string  _argval);
    explicit operator bool() const { return m_enabled; }
    [[nodiscard]] std::string as_string() const;
    [[nodiscard]] int as_integer() const;
};

std::map<std::string, argument_t> get_arguments(std::vector<option_t>& possible_options, int argc, char** argv);
void print_argument_help(const std::vector<option_t>& options);

#endif //AUTOVERIF_ARGUMENTS_H
