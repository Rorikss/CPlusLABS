#pragma once
#include <sstream>
#include <cstring>
#include <cstdio>
#include "Hamming.h"
#include "ByteAndBitOperations.h"
#include "FileOperation.h"

class HamArc {
public:
    HamArc(const std::string& name, uint8_t chunk_size=3);

    void AppendOneFile(const std::string& filename);
    void CreateArchive(const std::vector<std::string>& files);
    void Extract(const std::vector<std::string>& files);
    void Merge(const std::string& first_archive_name, const std::string& second_archive_name);
    void ListNames(std::ostream& os = std::cout);
    void DeleteFile(const std::string& file_name);

private:
    std::string ReadFileName(std::istream& input);
    uint64_t ReadFileSize(std::istream& input);
    void DecodeFile(const std::string& filename, std::istream& arch, uint64_t& current_pos_in_archive, uint64_t archive_size);
    void SkipFile(std::istream& arch, uint64_t& current_pos_in_archive, uint64_t archive_size);
    void WriteFileToArchive(const std::string& filename, std::ostream& output);
    void AddFileNameToArchive(std::ostream& arch, const std::string& path);
    void AddFileSizeToArchive(std::ostream& arch, uint64_t size);
    void AddThreeBytesToArch(char chunk[], std::ostream& arch);

    // helpers
    void CheckFileSize(uint64_t archive_size, uint64_t size, uint64_t current_pos_in_archive);
    bool Found(const std::string& cur_file_name, const std::vector<std::string>& files) const;
    bool IsHammingArchive(const std::string& line) const;

    std::string archiver_filename_;
    std::vector<std::string> files_;
    Hamming hamming_coder_;
    ByteAndBitOperations coder_;
    FileOperation file_operator_;
    const std::string kTemporaryArchiveName = "temporary.haf";
    const std::string kHammingSuffix = ".haf";
    const uint8_t kByte = 8;
    const uint8_t kFileSize = 8;

    uint8_t encode_chunk_size_;
    uint8_t decode_chunk_size_;
    uint8_t name_size_;
    uint8_t encoded_name_size_;
    uint8_t encoded_file_size_;
    uint8_t file_header_size_;
};
