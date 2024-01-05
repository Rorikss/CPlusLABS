#include "ByteAndBitOperations.h"

ByteAndBitOperations::ByteAndBitOperations(uint8_t encode_size, uint8_t decode_size, uint8_t free)
    : encoded_chunk_size_(encode_size)
    , decoded_chunk_size_(decode_size)
    , free_bits_(free)
    { bits_in_chunk_ = encode_size * kByte; }

std::vector<bool> ByteAndBitOperations::DecodeBytesToBoolVector(char* bits) {
    std::vector<bool> data = GenerateVector(bits, decoded_chunk_size_);
    data.erase(data.begin(), data.begin() + free_bits_);
    return data;
}

std::vector<bool> ByteAndBitOperations::EncodeBytesToBoolVector(char* chunk) const {
    return GenerateVector(chunk, encoded_chunk_size_);
}

std::vector<char> ByteAndBitOperations::MakeChunkOfBytes(const std::vector<bool>& decoded) const {
    std::vector<char> bytes;
    for (size_t i = 0; i < bits_in_chunk_; i += kByte) {
        uint8_t byte = MakeByte(decoded, i);
        bytes.push_back(byte);
    }
    return bytes;
}

std::string ByteAndBitOperations::MakeCharsOfName(const std::vector<bool>& decoded) const {
    std::string part_of_name;
    for (size_t i = 0; i < bits_in_chunk_; i += kByte) {
        char byte = MakeByte(decoded, i);
        part_of_name += static_cast<char>(byte);
    }
    return part_of_name;
}

std::vector<bool> ByteAndBitOperations::GenerateVector(char* chunk, uint32_t size) const {
    std::vector<bool> data;
    for (int32_t j = 0; j < size; ++j) {
        for (int32_t i = kByte - 1; i >= 0; --i) {
            data.push_back(chunk[j] & (1 << i));
        }
    }
    return data;
}

char ByteAndBitOperations::MakeByte(const std::vector<bool>& decoded, size_t i) const {
    uint8_t byte = 0;
    for (int j = kByte - 1; j >= 0; j--) {
        if (decoded[j + i]) {
            byte |= (1 << (kByte - j - 1));
        }
    }
    return byte;
}
