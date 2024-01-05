#pragma once
#include <iostream>
#include <vector>
#include <cstdint>

class Hamming {
public:
    Hamming(uint8_t size=24, uint8_t redundant=5);

    std::vector<bool> HammingDecode(const std::vector<bool>& received);
    std::vector<bool> HammingEncode(const std::vector<bool>& received);

private:
    uint8_t redundant_bits_number_;
    uint8_t chunk_size_;
};
