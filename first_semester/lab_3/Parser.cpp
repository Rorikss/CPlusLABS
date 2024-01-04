#include "Parser.h"

Parser::Parser(int argc, char **argv) {
    for (uint32_t i = 0; i < argc; ++i) {
        if (std::strcmp(argv[i], "--input") == 0 ||
            std::strcmp(argv[i], "-i") == 0) {
            CheckBounderies(i, argc);
            input = argv[++i];
        } else if (std::strcmp(argv[i], "--output") == 0 ||
                   std::strcmp(argv[i], "-o") == 0) {
            CheckBounderies(i, argc);
            output = argv[++i];
            if (output.back() != '/') output += "/";
        } else if (std::strcmp(argv[i], "--max-iter") == 0 ||
                   std::strcmp(argv[i], "-m") == 0) {
            CheckBounderies(i, argc);
            max_iter_ = Enumerate(argv[++i]);
        } else if (std::strcmp(argv[i], "--freq") == 0 ||
                   std::strcmp(argv[i], "-f") == 0) {
            CheckBounderies(i, argc);
            frequency_ = Enumerate(argv[++i]);
        }
    }
}

void Parser::CheckBounderies(uint32_t iter, uint32_t arg_num) {
    if (iter >= arg_num) {
        throw std::invalid_argument("No argument entered.");
    }
}

uint64_t Parser::Enumerate(char* number_argument) {
    for (int i = 0; i < strlen(number_argument); ++i) {
        if (!isdigit(number_argument[i])) {
            throw std::invalid_argument("Invalid number argument entered.");
        }
    }
    char *end;
    return std::strtol(number_argument, &end, 10);
}
