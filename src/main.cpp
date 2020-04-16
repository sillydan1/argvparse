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
#include "arguments.h"

int main(int argc, char** argv) {
    // Construct your cli flag options as a vector of option_t's
    // The --help flag gets added automatically
    std::vector<option_t> my_options = {
    //   long name   |short| argument requirement             | Help description
        {"flag"      , 'm', argument_requirement::NO_ARG      , "Flag with no argument"},
        {"input-file", 'f', argument_requirement::REQUIRE_ARG , "Flag with a required argument"},
        {"optimize"  , 'o', argument_requirement::OPTIONAL_ARG, "Flag with an optional argument"}
    };
    // Then call get_arguments
    auto cli_arguments = get_arguments(my_options, argc, argv);
    // If the help flag was provided, print out the help message
    if(cli_arguments["help"]) {
        std::cout << "example_usage is a program that shows how to use libargvparse. Belo are the possible options\nCopyright (C) 2020  Asger Gitz-Johansen\n"
                     "\n"
                     "    This program is free software: you can redistribute it and/or modify\n"
                     "    it under the terms of the GNU General Public License as published by\n"
                     "    the Free Software Foundation, either version 3 of the License, or\n"
                     "    (at your option) any later version.\n"
                     "\n"
                     "    This program is distributed in the hope that it will be useful,\n"
                     "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                     "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                     "    GNU General Public License for more details.\n"
                     "\n"
                     "    You should have received a copy of the GNU General Public License\n"
                     "    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n";
        print_argument_help(my_options);
        return EXIT_SUCCESS;
    }
    // You can test if an argument was provided by simply testing for the long name
    if(cli_arguments["flag"]) {
        // std::string flag_file = cli_arguments["flag"].as_string(); // Throws a runtime_exception, since there are no arguments on '--flag'
    }
    if(cli_arguments["input-file"]) {
        std::string input_file = cli_arguments["input-file"].as_string();
    }
    if(cli_arguments["optimize"]) {
        std::string optimize_strategy = cli_arguments["optimize"].as_string(); // Throws a runtime_exception if no flag was provided
    }
    return EXIT_SUCCESS;
}
