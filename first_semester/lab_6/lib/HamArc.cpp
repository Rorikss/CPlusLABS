#include "HamArc.h"


HamArc::HamArc(const std::string& name, uint8_t chunk_size) {
    archiver_filename_ = file_operator_.CreateArchiveName(name);
    encode_chunk_size_ = chunk_size;
    uint8_t number_of_redundants = 0;
    while ((1 << number_of_redundants) <= (encode_chunk_size_ * kByte + number_of_redundants + 1)) {
        number_of_redundants++;
    }
    uint8_t extra = (number_of_redundants / kByte) + (number_of_redundants % kByte > 0);
    decode_chunk_size_ = encode_chunk_size_ + extra;
    name_size_ = std::max(18, encode_chunk_size_ * 6);
    encoded_name_size_ = name_size_ + (name_size_ / encode_chunk_size_ + (name_size_ % encode_chunk_size_ > 0)) * extra;
    encoded_file_size_ = decode_chunk_size_ * (kFileSize / encode_chunk_size_ + (kFileSize % encode_chunk_size_ > 0));
    file_header_size_ = encoded_file_size_ + encoded_name_size_;
    uint8_t free = kByte * decode_chunk_size_ - (kByte * encode_chunk_size_ + number_of_redundants);
    hamming_coder_ = Hamming(encode_chunk_size_ * kByte, number_of_redundants);
    coder_ = ByteAndBitOperations(encode_chunk_size_, decode_chunk_size_, free);
    file_operator_ = FileOperation(decode_chunk_size_ * 8, free);
}

void HamArc::AppendOneFile(const std::string& filename) {
    std::ofstream output = file_operator_.WOpenFileInApp(archiver_filename_);
    WriteFileToArchive(filename, output);
}

void HamArc::CreateArchive(const std::vector<std::string>& files) {
    std::ofstream output = file_operator_.WOpenFileInBinary(archiver_filename_);
    for (const auto& name : files) {
        WriteFileToArchive(name, output);
    }
}

void HamArc::Extract(const std::vector<std::string>& files) {
    std::ifstream archive = file_operator_.ROpenFileInBinary(archiver_filename_);
    uint64_t archive_size = file_operator_.GetFileSize(archive);
    bool is_extract_all = files.empty();
    uint64_t current_position = 0;
    if (archive_size < file_header_size_) {
        throw std::runtime_error("error size");
    }
    while (current_position < archive_size && archive.good()) {
        std::string cur_file_name = ReadFileName(archive);
        if (Found(cur_file_name, files) || is_extract_all) {
            DecodeFile(cur_file_name, archive, current_position, archive_size);
        } else {
            SkipFile(archive, current_position, archive_size);
        }
        current_position += file_header_size_;
    }
}

void HamArc::Merge(const std::string& first_archive_name, const std::string& second_archive_name) {
    if (!IsHammingArchive(first_archive_name) || !IsHammingArchive(second_archive_name)) {
        throw std::runtime_error("Can concatenate only HamArc(s)");
    }
    std::ofstream archive = file_operator_.WOpenFileInBinary(archiver_filename_);
    std::ifstream input_1 = file_operator_.ROpenFileInBinary(first_archive_name);
    std::ifstream input_2 = file_operator_.ROpenFileInBinary(second_archive_name);

    file_operator_.WriteTillEndOfFile(input_1, archive);
    file_operator_.WriteTillEndOfFile(input_2, archive);
}

void HamArc::ListNames(std::ostream& os) {
    std::ifstream archive = file_operator_.ROpenFileInBinary(archiver_filename_);
    os << "Opening archive: \"" << archiver_filename_ << "\" ";
    uint64_t archive_size = file_operator_.GetFileSize(archive);
    os << "of size " << archive_size << "\n";
    uint64_t current_position = 0;
    while (current_position < archive_size) {
        if (archive_size < file_header_size_) {
            throw std::runtime_error("error size");
        }
        std::string cur_file_name = ReadFileName(archive);
        os << cur_file_name << "\n";
        SkipFile(archive, current_position, archive_size);
        current_position += file_header_size_;
    }
    os << "These are all files in current archive" << std::endl;
}

void HamArc::DeleteFile(const std::string& file_name) {
    std::ifstream archive = file_operator_.ROpenFileInBinary(archiver_filename_);
    uint64_t archive_size = file_operator_.GetFileSize(archive);
    std::ofstream temporary_arch = file_operator_.WOpenFileInBinary(kTemporaryArchiveName);
    uint64_t current_position = 0;
    bool is_found = false;
    while (current_position < archive_size) {
        if (archive_size < file_header_size_) {
            throw std::runtime_error("error size");
        }
        std::string cur_file_name = ReadFileName(archive);
        if (cur_file_name == file_name) {
            is_found = true;
            SkipFile(archive, current_position, archive_size);
        } else {
            AddFileNameToArchive(temporary_arch, cur_file_name);
            uint64_t size = ReadFileSize(archive);
            AddFileSizeToArchive(temporary_arch, size);
            current_position += size;
            for (size_t j = 0; j < size; j++) {
                char byte = archive.get();
                temporary_arch.put(byte);
            }
        }
        current_position += file_header_size_;
    }
    archive.close(); // закрыть надо, а то удалить нельзя
    temporary_arch.close();
    std::filesystem::remove(archiver_filename_.c_str()); // удалили то, что читали
    if (std::rename(kTemporaryArchiveName.c_str(), archiver_filename_.c_str()) != 0) { // подмена файла, так сказать (переименовали)
        std::cerr << "Error occurred while renaming file" << std::endl;
    }
    if (!is_found) {
        std::cerr << "No such file in present archive\n";
    }
}

std::string HamArc::ReadFileName(std::istream& input) {
    std::string file_name;
    for (size_t i = 0; i < encoded_name_size_; i += decode_chunk_size_) {
        char bits[decode_chunk_size_];
        for (size_t j = 0; j < decode_chunk_size_; ++j) {
            bits[j] = input.get();
        }
        std::vector<bool> bits_in_bools = coder_.DecodeBytesToBoolVector(bits);
        std::vector<bool> decoded = hamming_coder_.HammingDecode(bits_in_bools);
        file_name += coder_.MakeCharsOfName(decoded);
    }
    return file_operator_.ClearFileName(file_name);
}

uint64_t HamArc::ReadFileSize(std::istream& input) {
    uint64_t size;
    std::vector<char> bytes_of_number(kFileSize, 0);
    for (size_t i = 0; i < encoded_file_size_; i += decode_chunk_size_) {
        char bits[decode_chunk_size_];
        for (size_t j = 0; j < decode_chunk_size_; ++j) { // прочитали 4 байта
            bits[j] = input.get();
        }
        std::vector<bool> bits_in_bools = coder_.DecodeBytesToBoolVector(bits); // получили 29 булов ака битов
        std::vector<bool> decoded = hamming_coder_.HammingDecode(bits_in_bools); // декодировали 29 булов ака битов и получили 24 чистых бита
        std::vector<char> chunk_bytes_of_number = coder_.MakeChunkOfBytes(decoded);
        for (auto x : chunk_bytes_of_number) bytes_of_number.push_back(x);
    }
    int32_t shift = bytes_of_number.size() - kFileSize;
    for (auto it = bytes_of_number.begin() + shift; it != bytes_of_number.end(); ++it) {
        size = (size << kByte) | *it;
    }
    return size;
}

void HamArc::DecodeFile(const std::string& filename, std::istream& arch, uint64_t& current_pos_in_archive,
                        uint64_t archive_size) {
    std::ofstream output = std::ofstream(filename);
    uint64_t size = ReadFileSize(arch);
    CheckFileSize(archive_size, size, current_pos_in_archive);
    uint8_t current_position = 0;
    char current_byte;
    char current_chunk[decode_chunk_size_];
    for (uint64_t i = 0; i < size; ++i) {
        current_byte = arch.get();
        current_chunk[current_position] = current_byte;
        ++current_position;
        // Check if the chunk is full, then write it to the output stream
        if (current_position == decode_chunk_size_) {
            std::vector<bool> bits_in_bools = coder_.DecodeBytesToBoolVector(current_chunk); // получили 29 булей ака битов
            std::vector<bool> decoded = hamming_coder_.HammingDecode(bits_in_bools); // декодировали 29 булей ака битов и получили 24 чистых бита
            std::vector<char> three_bytes = coder_.MakeChunkOfBytes(decoded);
            for (auto symbol : three_bytes) {
                if (symbol) {
                    output << symbol;
                }
            }
            current_position = 0;  // Reset position for the next chunk
        }
    }
    current_pos_in_archive += size;
}

void HamArc::SkipFile(std::istream& arch, uint64_t& current_pos_in_archive, uint64_t archive_size) {
    uint64_t size = ReadFileSize(arch);
    CheckFileSize(archive_size, size, current_pos_in_archive);
    arch.seekg(size, std::ios::cur); // просто перемещаемся дальше
    current_pos_in_archive += size;
}

void HamArc::WriteFileToArchive(const std::string& filename, std::ostream& output) { // вот этот метод добавляет один файл в массив
    std::ifstream input = file_operator_.ROpenFileInBinary(filename);
    AddFileNameToArchive(output, filename); // пишем имя файла
    uint64_t file_size = file_operator_.GetFileSize(input);
    uint64_t new_file_size = file_size + ((encode_chunk_size_ - (file_size % encode_chunk_size_)) % encode_chunk_size_);
    new_file_size += (new_file_size / encode_chunk_size_);
    AddFileSizeToArchive(output, new_file_size); // пишем размер файла

    // пишем сами данные из файла
    char current_byte;
    char current_chunk[encode_chunk_size_];
    uint8_t current_position = 0;
    for (uint64_t i = 0; i < file_size; ++i) {
        current_byte = input.get();
        current_chunk[current_position] = current_byte;
        ++current_position;

        // Check if the chunk is full, then write it to the output stream
        if (current_position == encode_chunk_size_) {
            AddThreeBytesToArch(current_chunk, output);
            current_position = 0;  // Reset position for the next chunk
        }
    }
    if (current_position > 0) {
        for (size_t i = current_position; i < encode_chunk_size_; ++i) {
            current_chunk[i] = 0;
        }
        AddThreeBytesToArch(current_chunk, output);
    }
}

void HamArc::AddFileNameToArchive(std::ostream& arch, const std::string& path) {
    std::string file_name = file_operator_.FileName(path);
    uint8_t current_filename_size = file_name.size();
    if (current_filename_size > name_size_) {
        throw std::runtime_error("too long filename");
    }
    for (size_t i = 0; i < name_size_; i += encode_chunk_size_) {
        char chunk[encode_chunk_size_];
        for (int j = 0; j < encode_chunk_size_; ++j) {
            if (j + i < current_filename_size) {
                chunk[j] = file_name[i + j];
            } else {
                chunk[j] = '\0';
            }
        }
        AddThreeBytesToArch(chunk, arch);
    }
}

void HamArc::AddFileSizeToArchive(std::ostream& arch, uint64_t size) {
    uint8_t blank = (encode_chunk_size_ - kFileSize % encode_chunk_size_) % encode_chunk_size_;
    for (size_t i = 0; i < kFileSize + blank; i += encode_chunk_size_) {
        char chunk[encode_chunk_size_];
        for (int32_t j = 0; j < encode_chunk_size_; ++j) {
            if (i + j < blank) {
                chunk[j] = 0;
            } else {
                chunk[j] = (size >> ((kByte - (i + j - blank) - 1) * kByte));
            }
        }
        AddThreeBytesToArch(chunk, arch);
    }
}

void HamArc::AddThreeBytesToArch(char* chunk, std::ostream& arch) {
    std::vector<bool>bits = coder_.EncodeBytesToBoolVector(chunk);
    std::vector<bool>encoded = hamming_coder_.HammingEncode(bits);
    file_operator_.WriteChunkToFile(arch, encoded);
}

void HamArc::CheckFileSize(uint64_t archive_size, uint64_t size, uint64_t current_pos_in_archive) {
    if (size % decode_chunk_size_) {
        throw std::runtime_error("Size of file can't multiple to decode_chunk. Your file is encoded badly.");
    }
    if (archive_size - current_pos_in_archive < size) {
        throw std::runtime_error("to small archive size");
    }
}

bool HamArc::Found(const std::string& cur_file_name, const std::vector<std::string>& files) const {
    for (const auto& file : files) {
        if (file == cur_file_name) {
            return true;
        }
    }
    return false;
}

bool HamArc::IsHammingArchive(const std::string& line) const {
    if (line.length() >= kHammingSuffix.length()) {
        return (line.compare(line.length() - kHammingSuffix.length(), kHammingSuffix.length(), kHammingSuffix) == 0);
    } else {
        return false;
    }
}
