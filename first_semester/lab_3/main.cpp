#include "SandPile.h"
#include "Parser.h"

int main(int argc, char *argv[]) {
    Parser parameters(argc, argv);
    SandPile sand(parameters.input, parameters.output, parameters.frequency_, parameters.max_iter_);
    return EXIT_SUCCESS;
}
