//
// Created by Roriks on 5/1/2024.
//
#pragma once
#include <unordered_map>
#include <cstring>
#include <algorithm>
#include "VarInt.h"
#include "InfoStructures.h"

namespace SearchEngine {
class Indexer {
public:
    Indexer(const std::string& root, const std::string& out);
    void MakeIndex();

private:
    void TraverseDirectory();
    size_t ParseLine(std::string& line, size_t line_number);
    void WriteToFile();
    void OverflowCheck();

    std::filesystem::path root_directory_;
    std::filesystem::path output_;
    std::unordered_map<std::string, std::vector<FullInfo>> index_table_;
    std::vector<std::pair<std::string, size_t>> file_indexes_;
    size_t current_index_ = 0;
    bool is_chunked_;
    VarInt<size_t> var_int_;
    size_t kMaxTableSize = 1e12;
};
} // end of namespace
