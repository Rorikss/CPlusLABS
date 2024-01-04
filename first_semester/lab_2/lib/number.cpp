#include "number.h"
#include <cstring>

int2023_t from_int(int32_t number) {
    bool is_negative = (number < 0);
    if (is_negative) {
        number = abs(number);
    }
    int2023_t result;
    int next = 0;
    while (number) {
        result.digit[next] = number % int2023_t::kBase;
        next++;
        number /= int2023_t::kBase;
    }
    if (is_negative) {
        InvertSign(result);
    }
    return result;
}

int2023_t from_string(const char* buff) {
    int2023_t result = from_int(0);
    int num;
    bool is_negative = false;
    uint16_t i = 0;
    if (buff[0] == '-') {
        is_negative = true;
        i = 1;
    }
    for (; i < std::strlen(buff) - 1; i += 2) {
        num = 0;
        int2023_t mult_hundred = from_int(100);
        uint16_t power = int2023_t::kTenBase;
        for (uint16_t j = 0; j < 2; ++j) {
            num += (static_cast<uint16_t>(buff[i + j]) - '0') * power;
            power /= int2023_t::kTenBase;
        }
        result = result * mult_hundred;
        result = result + from_int(num);
    }
    if (i < std::strlen(buff)) {
        result = result * from_int(10);
        result = result + from_int(static_cast<uint16_t>(buff[strlen(buff) - 1]) - '0');
    }
    if (is_negative) {
        InvertSign(result);
    }

    return result;
}


int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    uint16_t carry = 0;
    for (uint16_t i = 0; i < int2023_t::kSize; ++i) {
        carry += lhs.digit[i] + rhs.digit[i];
        result.digit[i] = carry & (int2023_t::kBase - 1);
        carry >>= int2023_t::kByte;
    }

    return result;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t copy;
    for (uint16_t i = 0; i < int2023_t::kSize; ++i) {
        copy.digit[i] = ~rhs.digit[i];
    }
    return lhs + (copy + from_int(1));
}

void InvertSign(int2023_t& number){
    for (uint16_t i = 0; i < int2023_t::kSize; ++i) {
        number.digit[i] = ~number.digit[i];
    }
    number = number + from_int(1);
}

bool GetSign(const int2023_t& number){
    return ((number.digit[int2023_t::kSize - 1] >> (int2023_t::kByte - 1)) & 1);
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    bool is_left_negative = GetSign(lhs);
    bool is_right_negative = GetSign(rhs);
    int2023_t first_copy = lhs;
    int2023_t second_copy = rhs;
    if (is_left_negative) {
        InvertSign(first_copy);
    }
    if (is_right_negative) {
        InvertSign(second_copy);
    }
    int2023_t result;
    uint16_t carry = 0;
    for (uint16_t i = 0; i < int2023_t::kSize; ++i) {
        for (uint16_t j = 0; j < int2023_t::kSize - i; ++j) {
            carry += first_copy.digit[i] * second_copy.digit[j];
            carry += result.digit[i + j];
            result.digit[i + j] = carry & (int2023_t::kBase - 1);
            carry >>= int2023_t::kByte;
        }
    }
    if (is_right_negative != is_left_negative) {
        InvertSign(result);
    }
    return result;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs){
    for (int i = int2023_t::kSize - 1; i >= 0; i--) {
        if (lhs.digit[i] != rhs.digit[i]) {
            return lhs.digit[i] < rhs.digit[i];
        }
    }
    return false;
}

int2023_t MakeByteShift(int2023_t& number) {
    int2023_t tmp;
    uint16_t current_bit;
    for (uint16_t i = int2023_t::kLength; i > 0; --i) {
        current_bit = (number.digit[(i - 1) / int2023_t::kByte] >> ((i - 1) % int2023_t::kByte)) & 1;
        tmp.digit[i / int2023_t::kByte] ^= (current_bit << (i % int2023_t::kByte));
    }
    return tmp;
}

uint16_t GetFirstNotZero(int2023_t number) {
    for (int i = int2023_t::kSize - 1; i >= 0; --i) {
        for (int j = int2023_t::kByte - 1; j >= 0; --j) {
            if ((number.digit[i] >> j) & 1) {
                return int2023_t::kLength - (i * int2023_t::kByte + j);
            }
        }
    }
    return 0;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    bool is_left_negative = GetSign(lhs);
    bool is_right_negative = GetSign(rhs);
    int2023_t first_copy = lhs;
    int2023_t second_copy = rhs;
    if (is_left_negative) {
        InvertSign(first_copy);
    }
    if (is_right_negative) {
        InvertSign(second_copy);
    }
    int2023_t result;
    int2023_t dividing_value;
    for (int i = GetFirstNotZero(first_copy); i >= 0; --i) {
        dividing_value = MakeByteShift(dividing_value);
        if (first_copy.digit[i / int2023_t::kByte] & (1 << (i % int2023_t::kByte))) {
            dividing_value.digit[0] ^= 1;
        }
        if (!(dividing_value < second_copy)) {
            result.digit[i / int2023_t::kByte] ^= (1 << (i % int2023_t::kByte));
            dividing_value = dividing_value - second_copy;
        }
    }
    if (is_right_negative != is_left_negative && result != from_int(0)) {
        InvertSign(result);
    }

    return result;
}


bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (uint16_t i = 0; i < int2023_t::kSize; i++) {
        if (lhs.digit[i] != rhs.digit[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    uint16_t first_not_zero = GetFirstNotZero(value);
    for (uint16_t shifted_i = first_not_zero + 1; shifted_i > 0; --shifted_i) {
        for (uint16_t shifted_j = (int2023_t::kByte - 1); shifted_j > 0; --shifted_j) {
            uint8_t bit = (value.digit[shifted_i - 1] >> (shifted_j - 1)) & 1;
            stream << static_cast <int>(bit);
        }
    }
    if (first_not_zero == 0) {
        stream << 0;
    }
    return stream;
}