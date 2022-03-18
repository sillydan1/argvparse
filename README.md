# argvparse
A tiny C++ library for easier parsing of command line arguments

# Example Usage

Using argvparse is easy. You just define a list of desired arguments with a description and then you call `auto myargs = get_arguments(possible_arguments, argc, argv);`. You can then check if an argument has been provided by looking up the results like so: `myargs["long-name-look-up"]`. The result from `get_arguments` is a hashmap where the long name of the argument is the key.

Longer example. This example is also available as a build target: `make example_usage`

```C++
#include <iostream>
#include "arguments.h"

int main(int argc, char** argv) {
    // Construct your cli flag options as a vector of option_t's
    // The --help flag gets added automatically
    std::vector<option_t> my_options = {
    //   long name   |short| argument requirement             | Help description
        {"flag"      , 'a', argument_requirement::NO_ARG      , "Flag"},
        {"input-file", 'f', argument_requirement::REQUIRE_ARG , "Argument"},
        {"input-files",'F', argument_requirement::REQUIRE_ARG , "Argument that can be set multiple times (e.g. -F a -F b)"},
        {"optimize"  , 'o', argument_requirement::OPTIONAL_ARG, "Flag or Argument"}
    };
    // Then call get_arguments
    auto cli_arguments = get_arguments(my_options, argc, argv);
    // If the help flag was provided, print out the help message
    if(cli_arguments["help"]) {
        std::cout << "Copyright (C) 2020  Asger Gitz-Johansen\n"
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
                     "This is a program that shows how to use libargvparse. Below are the possible options\n\n";
        print_argument_help(my_options);
        return EXIT_SUCCESS;
    }
    // You can test if an argument was provided by simply testing for the long name
    if(cli_arguments["flag"]) {
        // std::string flag_file = cli_arguments["flag"].as_string(); // Throws a runtime_exception, since there are no arguments on '--flag'
    }
    if(cli_arguments["input-file"]) {
        std::cout << cli_arguments["input-file"].as_string() << std::endl;
    }
    if(cli_arguments["input-files"]) {
        for(auto& el : cli_arguments["input-files"].as_list())
            std::cout << el << " ";
        std::cout << std::endl;
    }
    if(cli_arguments["optimize"]) {
        std::string optimize_strategy = cli_arguments["optimize"].as_string(); // Throws a runtime_exception if no flag was provided
    }
    return EXIT_SUCCESS;
}
```

# Install

For compiling this library, a C++17 compatible compiler is required. The library also uses standard library components, so if you don't like that feel free to submit a pull request with custom 'unordered_map', 'vector' and 'std::for_each' implementations.  

```
mkdir bin && cd bin
cmake .. -DCMAKE_BUILD_TYPE=Release 
sudo make install
```

For uninstall

```
xargs rm < install_manifest.txt
```

Or just manually delete the files listed in `install_manifest.txt`. They are likely these:

```
/usr/local/lib/libargvparse.so
/usr/local/include/arguments.h
```

# Author(s)

 - [sillydan1](https://github.com/sillydan1)
