/**
 * Copyright (C) 2020 Asger Gitz-Johansen

   This file is part of libargvparse.

    libargvparse is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libargvparse is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libargvparse.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef AUTOVERIF_ARGUMENTS_H
#define AUTOVERIF_ARGUMENTS_H
#include <utility>
#include <vector>
#include <map>
#include <getopt.h>
#include <sstream>
#include <cstring>

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
    std::vector<std::string> argvals;
    char short_name;
    void as_check() const;
public:
    argument_t();
    argument_t(std::string  _long_name, char _short_name, std::string _argval);
    void add(const argument_t& other);
    explicit operator bool() const { return m_enabled; }
    [[nodiscard]] std::string as_string() const;
    [[nodiscard]] std::string as_string_or_default(const std::string&) const;
    [[nodiscard]] std::vector<std::string> as_list() const;
    [[nodiscard]] std::vector<std::string> as_list_or_default(const std::vector<std::string>&) const;
    [[nodiscard]] int as_integer() const;
    [[nodiscard]] int as_integer_or_default(const int&) const;
};

std::map<std::string, argument_t> get_arguments(std::vector<option_t>& possible_options, int argc, char** argv);
void print_argument_help(const std::vector<option_t>& options);
std::ostream& operator<<(std::ostream& stream, const std::vector<option_t>& options);
void add_help_option(std::vector<option_t>& options);

#endif //AUTOVERIF_ARGUMENTS_H
