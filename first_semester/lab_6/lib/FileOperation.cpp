#include "FileOperation.h"
#include <filesystem>

FileOperation::FileOperation(uint8_t enlarged_chunk_size, uint8_t free)
    : enlarged_chunk_size_(enlarged_chunk_size)
    , free_bits_(free)
    {}

uint64_t FileOperation::GetFileSize(std::istream& input) const {
    input.seekg(0, std::ios_base::end);
    std::streampos size = input.tellg();
    input.seekg(0, std::ios_base::beg);
    return size;
}

std::ifstream FileOperation::ROpenFileInBinary(const std::string& filename) {
    return OpenFile<std::ifstream>(filename, std::ios::binary);
}

std::ofstream FileOperation::WOpenFileInBinary(const std::string& filename) {
    return OpenFile<std::ofstream>(filename, std::ios::binary);
}

std::ofstream FileOperation::WOpenFileInApp(const std::string& filename) {
    return OpenFile<std::ofstream>(filename, std::ios::app);
}

void FileOperation::WriteTillEndOfFile(std::istream& input, std::ostream& output) {
    output << input.rdbuf();
}

void FileOperation::WriteChunkToFile(std::ostream& arch, const std::vector<bool>& bits) {
    std::vector<bool> answer = std::vector<bool>(free_bits_, false);
    for (auto bit : bits) {
        answer.push_back(bit);
    }
    for (int32_t j = 0; j < enlarged_chunk_size_; j += kByte) {
        char byte = 0;
        for (int32_t i = kByte - 1; i >= 0; i--) {
            if (answer[j + i]) {
                byte |= (1 << (kByte - i - 1));
            }
        }
        arch.put(byte);
    }
}

std::string FileOperation::FileName(const std::string& path) const  {
    std::filesystem::path filePath = path;
    return filePath.filename().string();
}

std::string FileOperation::CreateArchiveName(const std::string& name) {
    return name + ".haf";
}

std::string FileOperation::ClearFileName(const std::string& name) {
    for (size_t i = 0; i < name.size(); ++i) {
        if (name[i] == '\0') {
            return name.substr(0, i);
        }
    }
    return name;
}
