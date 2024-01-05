#include "Hamming.h"

Hamming::Hamming(uint8_t size, uint8_t redundant)
    : chunk_size_(size)
    , redundant_bits_number_(redundant)
    {}

std::vector<bool> Hamming::HammingDecode(const std::vector<bool>& received) {
    size_t size = received.size();
    std::vector<bool> decoded(size, false);
    for (size_t i = 0; i < size; ++i) {
        if ((i & (i + 1)) == 0) {
            decoded[i] = false;
        } else {
            decoded[i] = received[i];
        }
    }
    for (size_t i = 0; i < size; ++i) {
        if ((i & (i + 1)) == 0) {
            continue;
        }
        for (size_t j = 1; j <= size; j *= 2) {
            if ((j & (i + 1)) != 0 && decoded[i] == 1) {
                decoded[j - 1] = !decoded[j - 1];
            }
        }
    }
    int32_t error = -1;
    for (int i = 0; i < size; ++i) {
        if (decoded[i] != received[i]) {
            error += i + 1;
        }
    }
    if (error >= 0) {
        std::cerr << "Found mistake. Fixing it...\n";
        decoded[error] = !decoded[error];
    }
    std::vector<bool> result;
    for (size_t i = 0; i < size; ++i) {
        if ((i & (i + 1)) != 0) {
            result.push_back(decoded[i]);
        }
    }
    return result;
}

std::vector<bool> Hamming::HammingEncode(const std::vector<bool>& received) {
    std::vector<bool> encoded(redundant_bits_number_ + chunk_size_, false);
    size_t index = 0;
    for (size_t i = 0; i < redundant_bits_number_ + chunk_size_; ++i) {
        if ((i & (i + 1)) == 0) {
            encoded[i] = false;
        } else {
            encoded[i] = received[index++];
        }
    }
    for (size_t i = 0; i < redundant_bits_number_ + chunk_size_; ++i) {
        if ((i & (i + 1)) == 0) {
            continue;
        }
        for (size_t j = 1; j <= redundant_bits_number_ + chunk_size_; j *= 2) {
            if ((j & (i + 1)) != 0 && encoded[i] == 1) {
                encoded[j - 1] = !encoded[j - 1];
            }
        }
    }
    return encoded;
}
