#include <iostream>
#include "arguments.h"

int main(int argc, char** argv) {
    std::vector<option_t> engine_options = {
        {"input-model-file", 'm', argument_requirement::REQUIRE_ARG, "--input-model-file <FILE> is the file of the model to parse and perform verification on"},
        {"input-query-file", 'q', argument_requirement::REQUIRE_ARG, "--input-query-file <FILE> is the file containing queries"},
        {"optimize-model"  , 'o', argument_requirement::OPTIONAL_ARG, "Applies optimizations of provided type: LTL (default), SUPREMUM, HENRIK"}
    };
    auto cli_arguments = get_arguments(engine_options, argc, argv);
    if(cli_arguments["help"]) {
        std::cout << "tta-verify version 0.0a - LICENSE\n  USAGE: tta-verify --input /path/to/file.json etc.\n\n";
        print_argument_help(engine_options);
        return EXIT_SUCCESS;
    }

    if(cli_arguments["input-model-file"]) {
        std::string input_file = cli_arguments["input-model-file"].as_string();
    }
    if(cli_arguments["input-query-file"]) {
        std::string query_file = cli_arguments["input-query-file"].as_string();
    }
    if(cli_arguments["optimize-model"]) {
    }

    return EXIT_SUCCESS;
}
