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
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <iomanip>
#include "arguments.h"

argument_t::argument_t()
        : m_enabled{false}, long_name{}, argval{}, short_name{0} {}

argument_t::argument_t(std::string _long_name, char _short_name, std::string _argval)
        : m_enabled{true}, long_name{std::move(_long_name)}, argval{std::move(_argval)}, short_name{_short_name} {}

void argument_t::as_check() const {
    if(!m_enabled) {
        std::stringstream ss{}; ss << "Option " << long_name << " is not provided. Type --help or -h for help";
        throw std::runtime_error{ss.str()};
    }
    if(argval.empty()) {
        std::stringstream ss{}; ss << "Option " << long_name << " does not have an argument. Type --help or -h for help";
        throw std::runtime_error{ss.str()};
    }
}

std::string argument_t::as_string() const {
    as_check();
    return argval;
}

int argument_t::as_integer() const {
    as_check();
    return stoi(argval);
}

std::string get_optstring(const std::vector<option_t>& options) {
    std::stringstream optstring{":"}; // Start optstring with a ':' to enable optional arguments
    std::for_each(options.begin(), options.end(),
            [&optstring](const option_t& option) {
                optstring << option.short_option << (option.needs_argument >= argument_requirement::REQUIRE_ARG ? ":" : "");
            });
    return optstring.str();
}

std::unique_ptr<option> get_long_options(const std::vector<option_t>& options) {
    auto* long_options = new option[options.size()];
    int i = 0;
    std::for_each(options.begin(), options.end(),
            [&long_options, &i](const option_t& o) {
                long_options[i++] = static_cast<option>(o);
            });
    return std::unique_ptr<option>(long_options);
}

void add_help_option(std::vector<option_t>& options) {
    options.push_back({"help", 'h', argument_requirement::NO_ARG, "Print this message"});
}

void print_argument_help(const std::vector<option_t>& options) {
    std::for_each(options.begin(),options.end(),
            [](const option_t& o) {
                std::stringstream lft{},rght{};
                lft << " -" << o.short_option << ", --" << o.long_option;
                rght << "| " << o.description;
                printf("%-32s%-30s\n", lft.str().c_str(), rght.str().c_str());
            });
}

std::map<std::string, argument_t> get_arguments(std::vector<option_t>& possible_options, int argc, char** argv) {
    add_help_option(possible_options);
    std::map<std::string, argument_t> arguments{};
    int option_index                = 0;
    auto long_opts                  = get_long_options(possible_options);
    std::string optstring           = get_optstring(possible_options);
    int c = 0;
    while(c != -1) {
        c = getopt_long(argc, argv, optstring.c_str(), long_opts.get(), &option_index);
        auto element = std::find_if(possible_options.begin(), possible_options.end(),
                                 [&c](const option_t &o) { return o.short_option == c; });
        if (element != possible_options.end())
            arguments[element->long_option] = argument_t(
                    element->long_option,
                    element->short_option,
                    element->needs_argument >= argument_requirement::REQUIRE_ARG ? optarg == NULL ? "" : optarg : "");
    }
    return arguments;
}
