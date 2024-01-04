#pragma once
#include <cinttypes>
#include <iostream>


struct int2023_t {
    static const uint16_t kLength = 2023;
    static const uint16_t kBase = 256;
    static const uint8_t kSize = 253;
    static const uint8_t kByte = 8;
    static const uint8_t kTenBase = 10;

    uint8_t digit[kSize] {};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

void InvertSign(int2023_t& num);

bool GetSign(const int2023_t& number);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

int2023_t MakeByteShift(int2023_t& number);

uint16_t GetFirstNotZero(int2023_t number);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);