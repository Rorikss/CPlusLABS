#include <iostream>
#include "ArgParser.h"
#include "HamArc.h"

void SetParser(ArgumentParser::ArgParser& parser) {
    parser.AddStringArgument('c', "create").MultiValue().Positional();
    parser.AddStringArgument('f', "file");
    parser.AddFlag('l', "list");
    parser.AddStringArgument('x', "extract").MultiValue().Positional();
    parser.AddStringArgument('a', "append");
    parser.AddStringArgument('d', "delete");
    parser.AddIntArgument('s', "chunk_size");
    parser.AddStringArgument('A', "concatenate").MultiValue().Positional();
}

int main(int argc, char* argv[]) {
    std::cout << "!!PLEASE note that while modifying archive, chunk_size should be set depending on chunk_size, \nwith which this archive has been created\n";
    ArgumentParser::ArgParser parser("HamArcParser");
    SetParser(parser);
    parser.Parse(argc, argv);
    std::string name = "archive";
    uint8_t chunk = 3;
    std::vector<std::string> files_to_add;
    std::vector<std::string> files_to_extract;
    if (!parser.GetStringValue("file").empty()) {
        name = parser.GetStringValue("file");
    } else {
        throw std::runtime_error("No archive name provided.");
    }
    if (parser.GetIntValue("chunk_size")) {
        chunk = parser.GetIntValue("chunk_size");
    }
    HamArc archiver(name, chunk);
    if (!parser.GetStringValues("create").empty()) {
        files_to_add = parser.GetStringValues("create");
        files_to_add.erase(files_to_add.begin(), files_to_add.begin() + 1);
        archiver.CreateArchive(files_to_add);
    } else if (!parser.GetStringValue("append").empty()) {
        archiver.AppendOneFile(parser.GetStringValue("append"));
    } else if (!parser.GetStringValues("extract").empty()) {
        files_to_extract = parser.GetStringValues("extract");
        files_to_extract.erase(files_to_extract.begin(), files_to_extract.begin() + 1);
        archiver.Extract(files_to_extract);
    } else if (parser.GetFlag("list")) {
        archiver.ListNames();
    } else if (!parser.GetStringValue("delete").empty()) {
        archiver.DeleteFile(parser.GetStringValue("delete"));
    } else if (!parser.GetStringValue("concatenate").empty()) {
        std::vector<std::string> names = parser.GetStringValues("concatenate");
        archiver.Merge(names[1], names[2]);
    } else {
        throw std::invalid_argument("Invalid command");
    }
    return EXIT_SUCCESS;
}