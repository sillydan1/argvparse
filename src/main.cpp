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
#include "arguments.h"

int main(int argc, char** argv) {
    using namespace argvparse;
    // Construct your cli flag options as a vector of option_t's
    // The --help flag gets added automatically
    auto config = cli_config{{
            // long name |short|option requirement|argument requirement | Help description | <default value>
            {"flag"      , 'a', opt_req::optional, val_req::flag        , "flag"},
            {"input-file", 'f', opt_req::optional, val_req::required    , "argument"},
            {"input-files",'F', opt_req::optional, val_req::required    , "argument that can be set multiple times (-F a -F b)"},
            {"optimize"  , 'o', opt_req::optional, val_req::optional    , "flag or argument", "no"}
        },
        // Help header
        "Copyright (C) 2023  Asger Gitz-Johansen\n"
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
        "    along with this program.  If not, see <https://www.gnu.org/licenses/>.\n"
        "================================================================================\n"
        "This is a program that shows how to use libargvparse. Below are the possible options\n\n"
    };
    // Then call get_arguments
    auto cli_arguments = get_arguments(config.options, argc, argv);
    // If the help flag was provided, print out the help message
    if(cli_arguments["help"])
        return config.print_argument_help();
    // You can test if an argument was provided by simply testing for the long name
    if(cli_arguments["flag"])
        auto flag_file = cli_arguments["flag"].as<std::string>(); // Throws a runtime_exception, since there are no arguments on '--flag'
    if(cli_arguments["input-file"])
        std::cout << cli_arguments["input-file"].as<std::string>() << std::endl;
    if(cli_arguments["input-files"]) {
        for(auto& el : cli_arguments["input-files"].as_list<std::string>())
            std::cout << el << " ";
        std::cout << std::endl;
    }
    if(cli_arguments["optimize"])
        auto optimize_strategy = cli_arguments["optimize"].as<std::string>(); // Throws a runtime_exception if no flag was provided
    auto my_value = cli_arguments["input-file"].as<std::string>();
    std::cout << my_value << std::endl;
    return EXIT_SUCCESS;
}
