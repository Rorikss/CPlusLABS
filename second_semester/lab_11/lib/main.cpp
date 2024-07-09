//
// Created by Roriks on 5/1/2024.
//
#include "Indexer.h"
#include "Searcher.h"

using namespace SearchEngine;

void ParseIndexer(size_t argc, char* argv[]) {
    std::string root;
    std::string output = "output";
    for (int i = 2; i < argc; ++i) {
        std::string argument = argv[i];
        if (argument.substr(0, 5) == "root=") {
            root = argument.substr(5);
        } else if (argument.substr(0, 7) == "output=") {
            output = argument.substr(7);
        } else {
            throw std::runtime_error("Unknown argument entered");
        }
    }
    auto index = Indexer(root, output);
    index.MakeIndex();
}

void ParseSearcher(size_t argc, char* argv[]) {
    std::string index = "output";
    size_t k = 1;
    float b = 0.75;
    for (int i = 2; i < argc; ++i) {
        std::string argument = argv[i];
        if (argument.substr(0, 6) == "index=") {
            index = argument.substr(6);
        } else if (argument.substr(0, 2) == "k=") {
            k = std::stoll(argument.substr(2));
        } else if (argument.substr(0, 2) == "b=") {
            b = std::stof(argument.substr(2));
        } else {
            throw std::runtime_error("Unknown argument entered");
        }
    }
    auto searcher = Searcher(index, k, b);
    searcher.RunSearchApp();
}

int main(int argc, char* argv[]) {
    if (static_cast<std::string_view>(argv[1]) == "indexer") {
        ParseIndexer(argc, argv);
    } else if (static_cast<std::string_view>(argv[1]) == "searcher") {
        ParseSearcher(argc, argv);
    } else {
        throw std::runtime_error("Enter the app you want to use: indexer or searcher");
    }
    return EXIT_SUCCESS;
}
