#include <cstring>
#include <cstdint>
#include <iostream>

struct Parser {
    std::string input = "input.tsv";
    std::string output;
    uint32_t frequency_ = 0;
    uint64_t max_iter_ = 0;


    Parser(int argc, char **argv);
    uint64_t Enumerate(char* number_argument);
    void CheckBounderies(uint32_t iter, uint32_t arg_num);
};
