//
// Created by Roriks on 5/1/2024.
//

#include "Searcher.h"

SearchEngine::Searcher::Searcher(const std::string& index, size_t k, float b)
        : index_(index)
        , b_(b)
        , k_(k)
        {}

void SearchEngine::Searcher::RunSearchApp(std::istream& input, std::ostream& output) {
    auto file= OpenFile<std::ifstream>(index_.string() + ".txt", std::ios::in | std::ios::binary);
    RestoreData(file);
    output << "Enter '--exit' in order to exit the program.\n";
    output << "Enter '--help' in order to get help.\n";
    std::string command;
    std::getline(input, command);
    while (command != "--exit") {
        if (command == "--list") {
            ListFiles(output);
            std::getline(input, command);
            continue;
        }
        if (command == "--help") {
            PrintHelp(output);
            std::getline(input, command);
            continue;
        }
        auto search_list = ParseAsk(command);
        ImplementSearch(search_list->terms_, search_list->insertions_, output);
        std::getline(input, command);
    }
}

void SearchEngine::Searcher::TestOneQuery(std::istream& file, std::istream& input, std::ostream& output) {
    RestoreData(file);
    std::string command;
    std::getline(input, command);
    auto search_list = ParseAsk(command);
    ImplementSearch(search_list->terms_, search_list->insertions_, output);
}

void SearchEngine::Searcher::PrintHelp(std::ostream& output) {
    output << "Enter '--list' to see all files in the index directory.\n";
    output << "Connect all searched terms with operations 'AND' or 'OR'.\n";
    output << "You can use brackets to change the priority of search, but make sure you entered correct order of bkts.\n";
}

void SearchEngine::Searcher::ListFiles(std::ostream& output) {
    output << "Files in the index directory:\n";
    for (const auto& file : file_indexes_) {
        output << file.first << "\n";
    }
}

void SearchEngine::Searcher::RestoreData(std::istream& file) {
    size_t file_indexes_size, index_table_size;
    file >> file_indexes_size >> index_table_size;

    ReadFileIndexes(file, file_indexes_size);
    ReadIndexTable(file, index_table_size);
}

void SearchEngine::Searcher::ReadFileIndexes(std::istream& file, size_t file_indexes_size) {
    std::string key;
    size_t size;
    for (size_t i = 0; i < file_indexes_size; ++i) {
        file >> key >> size;
        avg_length_ += (static_cast<double>(size) / static_cast<double>(file_indexes_size));
        file_indexes_.push_back({key, size});
    }
}

void SearchEngine::Searcher::ReadIndexTable(std::istream& file, size_t index_table_size) {
    std::string key;
    size_t size;
    uint8_t byte;
    size_t n;
    for (size_t iter = 0; iter < index_table_size; ++iter) {
        file >> key >> size;
        for (size_t i = 0; i < size; ++i) {
            file >> n;
            if (n == 0) { // left in case index was damaged
                continue;
            }
            std::vector<uint8_t> values;
            byte = file.get();
            for (size_t j = 0; j < n; ++j) {
                byte = file.get();
                values.push_back(byte);
            }
            std::vector<size_t> decoded_values = var_int_.Decompress(values);
            index_table_[key].push_back(FullInfo(decoded_values[0],
                                                 std::vector(decoded_values.begin() + 1,decoded_values.end())));
        }
    }
}

std::shared_ptr<SearchEngine::TermSearch> SearchEngine::Searcher::ParseAsk(const std::string& line) {
    SyntaxParser parser;
    parser.MakeQuery(line);
    std::vector<std::string> order = parser.MakeParseOrder();
    std::stack<std::shared_ptr<TermSearch>> stack;
    for (const auto& term : order) {
        if (term == "AND" || term == "OR") {
            auto rhs = stack.top(); stack.pop();
            auto lhs = stack.top(); stack.pop();
            if (term == "AND") {
                stack.push(std::make_shared<AND>(AND(lhs, rhs)));
            } else {
                stack.push(std::make_shared<OR>(OR(lhs, rhs)));
            }
            continue;
        }

        stack.push(std::make_shared<WordTerm>(WordTerm(term, index_table_[term])));
    }
    auto search_list = stack.top();
    stack.pop();
    return search_list;
}

void SearchEngine::Searcher::ImplementSearch(const std::vector<std::string>& terms, const std::vector<FullInfo>& search_list,
                                        std::ostream& output) {
    std::priority_queue<FileInfo, std::vector<FileInfo>,
            decltype([](const FileInfo& a, const FileInfo& b) {
                return a.score_ < b.score_; })> kmax_heap;
    double mini = kMax;
    for (const auto& item : search_list) {
        double score = BM25ForTerm(terms, item);
        if (kmax_heap.size() < k_ || score > mini) {
            kmax_heap.push(FileInfo(score, item));
            mini = std::min(score, mini);
        }
    }
    PrintFirstK(kmax_heap, output);
}

void SearchEngine::Searcher::PrintFirstK(auto& heap, std::ostream& output) {
    if (heap.size() == 0) {
        output << "No such word found\n";
        return;
    }
    output << "Found your search in files:\n";
    size_t n = heap.size();
    for (size_t i = 0; i < std::min(k_, n); ++i) {
        auto search = heap.top();
        auto filename = file_indexes_[search.file_info_.file_index_].first;
        heap.pop();
        output << std::to_string(i + 1) << ". " << filename << " on lines: ";
        for (auto number : search.file_info_.line_numbers_) {
            output << number + 1 << " ";
        }
        output << std::endl;
    }
}

double SearchEngine::Searcher::BM25ForTerm(const std::vector<std::string>& words, const SearchEngine::FullInfo& file) {
    double score = 0;

    for (const auto& word: words) {
        double all_files = file_indexes_.size();
        double files_with_word = index_table_[word].size();
        double words_in_current_file = file_indexes_[file.file_index_].second;
        double necessary_words = file.line_numbers_.size();

        double tf = necessary_words / words_in_current_file;
        double idf = log((all_files - files_with_word + kReinforced) / (files_with_word + kReinforced));
        score += idf * (tf * (k_ + 1)) / (tf + k_ * (1 - b_ + b_ * all_files / avg_length_));
    }

    return score;
}
