# argvparse
A tiny C++ library for easier parsing of command line arguments

# Example Usage

Using argvparse is easy. You just define a list of desired arguments with a description and then you call `auto myargs = get_arguments(possible_arguments, argc, argv);`. You can then check if an argument has been provided by looking up the results like so: `myargs["long-name-look-up"]`. The result from `get_arguments` is a hashmap where the long name of the argument is the key.

Longer example

```C++
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
        std::cout << "example_usage is a program that shows how to use libargvparse. Here are the possible options:\n\n";
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
```

# Install

For compiling this library, a C++17 compatible compiler is required. The library also uses standard library components, so if you don't like that feel free to submit a pull request with custom 'unordered_map', 'vector' and 'std::for_each' implementations.  
