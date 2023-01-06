/**
 * Copyright (C) 2023 Asger Gitz-Johansen

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
#ifndef ARGVPARSE_ARGUMENTS_H
#define ARGVPARSE_ARGUMENTS_H
#include <utility>
#include <vector>
#include <map>
#include <getopt.h>
#include <sstream>
#include <cstring>
#include <optional>
#include <variant>
#include <any>

namespace argvparse {
    /// Commandline option provision requirement enumeration
    enum class opt_req {
        /// this option can be safely ignored
        optional,
        /// this option must be provided
        required,
    };

    /// Commandline option value requirement enumeration
    enum class val_req {
        /// required that no argument is provided
        flag = 0,
        /// required that an argument is provided
        required,
        /// an argument may or may not be provided
        optional,
    };

    /// Commandline option data structure
    struct option_t {
        /// Long version of the option e.g. --foo-bar
        const char *long_option;
        /// Short version of the option. e.g. -f
        char short_option;
        /// Is this option required for your program to function?
        opt_req required;
        /// Does this option require an argument?
        val_req needs_argument;
        /// Help message description of this option
        std::string description;
        /// If this option is not provided, this is the default value.
        std::optional<std::string> default_value;
        /// Constructor for options with no default value
        option_t(const char* long_option, char short_option, opt_req required, val_req needs_argument, const std::string_view& description)
                : long_option{long_option}, short_option{short_option}, required{required}, needs_argument{needs_argument}, description{description}, default_value{} {}
        /// Constructor for options with a default value
        option_t(const char* long_option, char short_option, opt_req required, val_req needs_argument, const std::string_view& description, const std::string& default_value)
                : long_option{long_option}, short_option{short_option}, required{required}, needs_argument{needs_argument}, description{description}, default_value{default_value} {}
        /// Cast to gnu getopt option
        explicit operator ::option() const { return {long_option, (int) needs_argument, nullptr, short_option}; }
    };

    struct cli_config {
        std::vector<option_t> options;
        std::string_view help_header{};
        auto print_argument_help(std::ostream& os = std::cout) const -> int;
    };

    class argument {
        bool provided;
        std::string long_name;
        char short_name;
        std::vector<std::any> values;

        void as_check() const;
    public:
        argument();
        argument(std::string long_name, char short_name, std::optional<std::any> value);
        void add(const argument &other);
        explicit operator bool() const { return provided; }

        template<typename T> auto as() const -> T {
            return as_indexed<T>(0);
        }
        template<typename T> auto as_indexed(size_t index) const -> T {
            as_check();
            if(index >= values.size())
                throw std::range_error("argument index out of range");
            return std::any_cast<T>(values[index]);
        }
        template<typename T> auto as_list() const -> std::vector<T> {
            as_check();
            std::vector<T> elements{};
            elements.reserve(values.size());
            for(size_t i = 0; i < values.size(); i++)
                elements.push_back(as_indexed<T>(i));
            return elements;
        }
    };

    auto get_arguments(std::vector<option_t> &possible_options, int argc, char **argv) -> std::map<std::string, argument>;
    auto operator<<(std::ostream &stream, const std::vector<option_t> &options) -> std::ostream&;
    void add_help_option(std::vector<option_t> &options);
}

#endif //ARGVPARSE_ARGUMENTS_H
