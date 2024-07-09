//
// Created by Roriks on 5/2/2024.
//
#pragma once
#include <vector>
#include <type_traits>
#include <cstdint>
#include <concepts>
#include <iostream>

namespace SearchEngine {

template<typename T> requires(std::is_integral<T>::value)
class VarInt {
public:
    std::vector<uint8_t> Compress(T num, const std::vector<T>& input) {
        std::vector<uint8_t> result;
        CompressNumber(num, result);
        for (size_t i = 0; i < input.size(); ++i) {
            T value = input[i] - (i > 0 ? input[i - 1] : 0);
            if (value || !i) {
                CompressNumber(value, result);
            }
        }
        return result;
    }

    std::vector<T> Decompress(const std::vector<uint8_t>& input) {
        std::vector<T> result;
        auto byte = input.begin();

        while (byte != input.end()) {
            T value = *byte & kMaxChunk;
            size_t shift = kShiftSize;
            while (byte != input.end() && *byte >= kContinuedValue) {
                byte++;
                value += ((*byte & kMaxChunk) << shift);
                shift += kShiftSize;
            }
            if (byte != input.end()) {
                byte++;
            }
            result.push_back(value + (result.size() < 2 ? 0 : result.back()));
        }

        return result;
    }

private:
    void CompressNumber(T number, std::vector<uint8_t>& result) {
        while (number > kMaxChunk) {
            uint8_t byte = (number & kMaxChunk) | kContinuedValue;
            result.push_back(byte);
            number >>= kShiftSize;
        }
        uint8_t byte = number;
        result.push_back(byte);
    }

    size_t kShiftSize = 7;
    size_t kMaxChunk = 0x7F;
    size_t kContinuedValue = 0x80;
};
}
