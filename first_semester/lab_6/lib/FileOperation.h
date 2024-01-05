#pragma once
#include <fstream>
#include <filesystem>
#include <vector>
#include <cstdint>

class FileOperation {
public:
    FileOperation(uint8_t enlarged_chunk_size=32, uint8_t free=3);

    uint64_t GetFileSize(std::istream& input) const;
    std::ifstream ROpenFileInBinary(const std::string& filename);
    std::ofstream WOpenFileInBinary(const std::string& filename);
    std::ofstream WOpenFileInApp(const std::string& filename);
    void WriteTillEndOfFile(std::istream& input, std::ostream& output);
    void WriteChunkToFile(std::ostream& arch, const std::vector<bool>& bits);
    std::string FileName(const std::string& path) const;
    std::string CreateArchiveName(const std::string& name);
    std::string ClearFileName(const std::string& name);

private:
    template <typename T>
    T OpenFile(const std::string& filename, std::ios_base::openmode mode) {
        T file(filename, mode);
        if (!file) {
            throw std::runtime_error("Cannot open " + filename + " file");
        }
        return file;
    }

    uint8_t enlarged_chunk_size_;
    uint8_t free_bits_;
    static const uint8_t kByte = 8;
};
