//
// Created by Roriks on 5/1/2024.
//
#pragma once
#include <unordered_map>
#include <cmath>
#include <queue>
#include "InfoStructures.h"
#include "VarInt.h"
#include "SyntaxParser.h"
#include "TermSearch.h"

namespace SearchEngine {
class Searcher {
public:
    Searcher(const std::string& index, size_t k, float b);
    void RunSearchApp(std::istream& input = std::cin, std::ostream& output = std::cout);
    void TestOneQuery(std::istream& file, std::istream& input, std::ostream& output);

private:
    void PrintHelp(std::ostream& output);
    void ListFiles(std::ostream& output);
    void RestoreData(std::istream& file);
    void ReadFileIndexes(std::istream& file, size_t file_indexes_size);
    void ReadIndexTable(std::istream& file, size_t index_table_size);
    std::shared_ptr<TermSearch> ParseAsk(const std::string& line);
    void ImplementSearch(const std::vector<std::string>& terms, const std::vector<FullInfo>& search_list, std::ostream& output);
    void PrintFirstK(auto& heap, std::ostream& output);
    double BM25ForTerm(const std::vector<std::string>& words, const FullInfo& file);

    double b_;
    double kReinforced = 0.5;
    double kMax = 1e9;
    size_t k_;
    std::filesystem::path index_;
    double avg_length_;
    std::unordered_map<std::string, std::vector<FullInfo>> index_table_;
    std::vector<std::pair<std::string, size_t>> file_indexes_;
    VarInt<size_t> var_int_;
};
} // end of nameapce
