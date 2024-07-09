#pragma once
#include <cstdint>
#include <vector>
#include <fstream>
#include <filesystem>

namespace SearchEngine {

struct FullInfo {
    size_t file_index_;
    std::vector<size_t> line_numbers_;

    FullInfo(size_t index, size_t line) : file_index_(index), line_numbers_({line}) {}
    FullInfo(size_t index, const std::vector<size_t>& v) : file_index_(index), line_numbers_(v) {}
};

struct FileInfo {
    FullInfo file_info_;
    double score_;

    FileInfo(double score, const FullInfo& data) : score_(score), file_info_(data) {}
};

template <typename T>
T OpenFile(const std::string& filename, std::ios_base::openmode mode) {
    T file(filename, mode);
    if (!file) {
        throw std::runtime_error("Cannot open " + filename + " file");
    }
    return file;
}

}
