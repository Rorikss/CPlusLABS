#pragma once
#include <iostream>
#include <cstdint>
#include <vector>

class ByteAndBitOperations {
public:
    ByteAndBitOperations(uint8_t encode_size=3, uint8_t decode_size=4, uint8_t free=3);

    std::vector<bool> DecodeBytesToBoolVector(char bits[]);
    std::vector<bool> EncodeBytesToBoolVector(char chunk[]) const;
    std::vector<char> MakeChunkOfBytes(const std::vector<bool>& decoded) const;
    std::string MakeCharsOfName(const std::vector<bool>& decoded) const;

private:
    std::vector<bool> GenerateVector(char chunk[], uint32_t size) const;
    char MakeByte(const std::vector<bool>& decoded, size_t i) const;

    uint8_t encoded_chunk_size_;
    uint8_t decoded_chunk_size_;
    uint8_t bits_in_chunk_;
    uint8_t free_bits_;
    static const uint8_t kByte = 8;
};
