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

argument_t::argument_t()
        : m_enabled{false}, long_name{}, argvals{}, short_name{0} {}

argument_t::argument_t(std::string _long_name, char _short_name, std::string _argval)
        : m_enabled{true}, long_name{std::move(_long_name)}, argvals{std::move(_argval)}, short_name{_short_name} {

}

void argument_t::add(const argument_t& other) {
    long_name = other.long_name;
    short_name = other.short_name;
    m_enabled = other.m_enabled;
    for(auto& a : other.argvals)
        argvals.push_back(a);
}

void argument_t::as_check() const {
    if(!m_enabled) {
        std::stringstream ss{}; ss << "Option " << long_name << " is not provided. Type --help or -h for help";
        throw std::runtime_error{ss.str()};
    }
    if(argvals.empty()) {
        std::stringstream ss{}; ss << "Option " << long_name << " does not have an argument. Type --help or -h for help";
        throw std::runtime_error{ss.str()};
    }
}

std::string argument_t::as_string() const {
    as_check();
    return argvals[0];
}

std::string argument_t::as_string_or_default(const std::string& default_value) const {
    if(m_enabled)
        return as_string();
    return default_value;
}

std::vector<std::string> argument_t::as_list() const {
    as_check();
    return argvals;
}

std::vector<std::string> argument_t::as_list_or_default(const std::vector<std::string>& default_value) const {
    if(m_enabled)
        return as_list();
    return default_value;
}

int argument_t::as_integer() const {
    as_check();
    return stoi(argvals[0]);
}

int argument_t::as_integer_or_default(const int& default_value) const {
    if(m_enabled)
        return as_integer();
    return default_value;
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

static bool option_len_compare(const option_t& a, const option_t& b) {
    return strlen(a.long_option) < strlen(b.long_option);
}

size_t get_max_option_length(const std::vector<option_t>& options) {
    return strlen(std::max_element(options.begin(), options.end(), option_len_compare)->long_option);
}

void print_argument_help(const std::vector<option_t>& options) {
    std::cout << options;
}

std::ostream& operator<<(std::ostream& stream, const std::vector<option_t>& options) {
    auto extra_padding = 13; // "--" on long option + " -x, " on short options + " <val> " on arguments
    auto max_left = get_max_option_length(options) + extra_padding;
    std::for_each(options.begin(),options.end(),
                  [&max_left, &stream](const option_t& o) {
                      std::stringstream lft{},rght{};
                      lft << " -" << o.short_option << ", --" << o.long_option;
                      if(o.needs_argument > argument_requirement::NO_ARG)
                          lft << " <val>";
                      rght << "| " << o.description;

                      auto l = TextFlow::Column(lft.str()).width(max_left);
                      auto r = TextFlow::Column(rght.str()).width(80-max_left-1);
                      auto layout = l + TextFlow::Spacer(1) + r;
                      stream << layout << "\n";
                  });
    return stream;
}

void sort_short_options(std::vector<option_t>& options) {
    std::sort(options.begin(), options.end(), [](const option_t& a, const option_t& b){
        return a.short_option < b.short_option;
    });
}

void sort_long_options(std::vector<option_t>& options) {
    std::sort(options.begin(), options.end(), [](const option_t& a, const option_t& b){
        return std::string(a.long_option) < b.long_option;
    });
}

void ensure_non_colliding_options(std::vector<option_t>& options) {
    sort_short_options(options);
    auto found = std::adjacent_find(options.begin(), options.end(), [](const option_t& a, const option_t& b){
        return a.short_option == b.short_option;
    });
    if(found != options.end())
        throw std::logic_error(std::string("Duplicate short option: ") + found->short_option);

    sort_long_options(options);
    found = std::adjacent_find(options.begin(), options.end(), [](const option_t& a, const option_t& b){
        return strcmp(a.long_option, b.long_option) == 0;
    });
    if(found != options.end())
        throw std::logic_error(std::string("Duplicate long option: ") + found->long_option);
}

std::map<std::string, argument_t> get_arguments(std::vector<option_t>& possible_options, int argc, char** argv) {
    add_help_option(possible_options);
    auto opts = possible_options;
    ensure_non_colliding_options(opts);
    std::map<std::string, argument_t> arguments{};
    int option_index = 0;
    auto long_opts = get_long_options(possible_options);
    auto optstring = get_optstring(possible_options);
    int c = 0;
    while(c != -1) {
        c = getopt_long(argc, argv, optstring.c_str(), long_opts.get(), &option_index);
        auto element = std::find_if(possible_options.begin(), possible_options.end(),
                                 [&c](const option_t &o) { return o.short_option == c; });
        if (element != possible_options.end())
            arguments[element->long_option].add(argument_t(
                    element->long_option,
                    element->short_option,
                    element->needs_argument >= argument_requirement::REQUIRE_ARG ? optarg == NULL ? "" : optarg : ""));
    }
    return arguments;
}
