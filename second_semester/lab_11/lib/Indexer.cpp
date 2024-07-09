//
// Created by Roriks on 5/1/2024.
//
#include "Indexer.h"

SearchEngine::Indexer::Indexer(const std::string& root, const std::string& out)
        : root_directory_(root)
        , output_(out)
        , is_chunked_(false)
        {}

void SearchEngine::Indexer::MakeIndex() {
    TraverseDirectory();
    if (!index_table_.empty()) {
        WriteToFile();
    }
}

void SearchEngine::Indexer::TraverseDirectory() {
    for (const auto& entry: std::filesystem::recursive_directory_iterator(root_directory_)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        auto file = OpenFile<std::ifstream>(entry.path().string(), std::ios::in);
        std::string line;
        size_t number_of_words = 0;
        size_t line_number = 0;
        while (std::getline(file, line)) {
            number_of_words += ParseLine(line, line_number);
            ++line_number;
        }
        file_indexes_.push_back({entry.path().filename().string(), number_of_words});
        ++current_index_;
    }
    OverflowCheck();
}

size_t SearchEngine::Indexer::ParseLine(std::string& line, size_t line_number) {
    size_t number_of_words = 0;
    char* token = std::strtok(line.data(), " ,?!*(){}:.<>;'-\"#\t|&!~+-[]_=`/\\");
    while (token != nullptr) {
        std::string word = token;
        number_of_words++;
        std::transform(word.begin(), word.end(), word.begin(), tolower);
        if (index_table_.find(word) != index_table_.end()
            && index_table_[word].back().file_index_ == current_index_) {
            index_table_[word].back().line_numbers_.push_back(line_number);
        } else {
            index_table_[word].push_back(FullInfo{current_index_, line_number});
        }
        token = std::strtok(nullptr, " ,?!*(){}:.<>;'-\"#\t|&!~+-[]_=`/\\");
    }
    return number_of_words;
}

void SearchEngine::Indexer::WriteToFile() {
    std::string number = (is_chunked_ ? std::to_string(current_index_) : "");
    auto file = OpenFile<std::ofstream>(output_.string() + number + ".txt", std::ios::binary);
    file << file_indexes_.size() << ' ' << index_table_.size() << ' ';
    for (const auto& name : file_indexes_) {
        file << name.first << ' ' << name.second << ' ';
    }
    for (const auto& pair : index_table_) {
        file << pair.first << ' ' << pair.second.size() << ' ';
        for (const auto& file_info : pair.second) {
            std::vector<uint8_t> bytes = var_int_.Compress(file_info.file_index_, file_info.line_numbers_);
            file << bytes.size() << ' ';
            for (auto c : bytes) {
                file << c;
            }
            file << ' ';
        }
    }
}

void SearchEngine::Indexer::OverflowCheck() {
    if (index_table_.size() >= kMaxTableSize) {
        is_chunked_ = true;
        WriteToFile();
        index_table_.clear();
        file_indexes_.clear();
    }
}
