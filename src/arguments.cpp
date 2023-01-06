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
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <iomanip>
#include "arguments.h"
#include <TextFlow.hpp>

namespace argvparse {
    argument::argument()
            : provided{false}, long_name{}, values{}, short_name{0} {}

    argument::argument(std::string long_name, char short_name, std::optional<std::any> value)
            : provided{true}, long_name{std::move(long_name)}, values{}, short_name{short_name} {
        if(value.has_value())
            values.push_back(value.value());
    }

    void argument::add(const argument& other) {
        long_name = other.long_name;
        short_name = other.short_name;
        provided = other.provided;
        for (auto &a: other.values)
            values.push_back(a);
    }

    void argument::as_check() const {
        if (!provided) {
            std::stringstream ss{};
            ss << "option " << long_name << " is not provided, see --help or -h for help";
            throw std::runtime_error{ss.str()};
        }
        if (values.empty()) {
            std::stringstream ss{};
            ss << "option " << long_name << " does not have an argument, see --help or -h for help";
            throw std::runtime_error{ss.str()};
        }
    }

    auto get_optstring(const std::vector<option_t>& options) -> std::string {
        std::stringstream optstring{":"}; // Start optstring with a ':' to enable optional arguments
        std::for_each(options.begin(), options.end(),
                      [&optstring](const option_t &option) {
                          optstring << option.short_option
                                    << (option.needs_argument >= val_req::required ? ":" : "");
                      });
        return optstring.str();
    }

    auto get_long_options(const std::vector<option_t>& options) -> std::unique_ptr<option> {
        auto *long_options = new option[options.size()];
        int i = 0;
        std::for_each(options.begin(), options.end(),
                      [&long_options, &i](const option_t &o) {
                          long_options[i++] = static_cast<option>(o);
                      });
        return std::unique_ptr<option>(long_options);
    }

    void add_help_option(std::vector<option_t>& options) {
        options.emplace_back("help", 'h', opt_req::optional, val_req::flag, "Print this message");
    }

    static auto option_len_compare(const option_t& a, const option_t& b) -> bool {
        return strlen(a.long_option) < strlen(b.long_option);
    }

    auto get_max_option_length(const std::vector<option_t>& options) -> size_t {
        return strlen(std::max_element(options.begin(), options.end(), option_len_compare)->long_option);
    }

    auto cli_config::print_argument_help(std::ostream& os) const -> int {
        os << help_header << options;
        return 0;
    }

    auto operator<<(std::ostream &stream, const std::vector<option_t>& options) -> std::ostream& {
        auto extra_padding = 13; // "--" on long option + " -x, " on short options + " <val> " on arguments
        auto max_left = get_max_option_length(options) + extra_padding;
        std::for_each(options.begin(), options.end(),
                      [&max_left, &stream](const option_t& o) {
                          std::stringstream left{}, right{};
                          left << " -" << o.short_option << ", --" << o.long_option;
                          if (o.needs_argument > val_req::flag)
                              left << " <val>";
                          right << o.description;
                          if(o.default_value.has_value())
                            right << " default: " << o.default_value.value();

                          auto l = TextFlow::Column(left.str()).width(max_left);
                          auto r = TextFlow::Column(right.str()).width(80 - max_left - 1);
                          auto layout = l + TextFlow::Spacer(1) + r;
                          stream << layout << "\n";
                      });
        return stream;
    }

    void sort_short_options(std::vector<option_t>& options) {
        std::sort(options.begin(), options.end(), [](const option_t &a, const option_t &b) {
            return a.short_option < b.short_option;
        });
    }

    void sort_long_options(std::vector<option_t>& options) {
        std::sort(options.begin(), options.end(), [](const option_t &a, const option_t &b) {
            return std::string(a.long_option) < b.long_option;
        });
    }

    void ensure_non_colliding_options(std::vector<option_t>& options) {
        sort_short_options(options);
        auto found = std::adjacent_find(options.begin(), options.end(), [](const option_t &a, const option_t &b) {
            return a.short_option == b.short_option;
        });
        if (found != options.end())
            throw std::logic_error(std::string("duplicate short option: ") + found->short_option);

        sort_long_options(options);
        found = std::adjacent_find(options.begin(), options.end(), [](const option_t &a, const option_t &b) {
            return strcmp(a.long_option, b.long_option) == 0;
        });
        if (found != options.end())
            throw std::logic_error(std::string("duplicate long option: ") + found->long_option);
    }

    auto get_arguments(std::vector<option_t>& possible_options, int argc, char** argv) -> std::map<std::string, argument> {
        add_help_option(possible_options);
        auto opts = possible_options;
        ensure_non_colliding_options(opts);
        std::map<std::string, argument> arguments{};
        int option_index = 0;
        auto long_opts = get_long_options(possible_options);
        auto optstring = get_optstring(possible_options);
        int c = 0;
        while (c != -1) {
            c = getopt_long(argc, argv, optstring.c_str(), long_opts.get(), &option_index);
            auto element = std::find_if(possible_options.begin(), possible_options.end(),
                                        [&c](const option_t &o) { return o.short_option == c; });
            if (element != possible_options.end())
                arguments[element->long_option].add(argument{
                        element->long_option,
                        element->short_option,
                        element->needs_argument >= val_req::required ?
                            optarg == nullptr ?
                                "" : optarg : ""});
        }
        return arguments;
    }
}
