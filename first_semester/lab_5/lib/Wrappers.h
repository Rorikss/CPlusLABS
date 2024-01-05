#pragma once
#include <iostream>
#include <cstdint>

void CheckNumberIsInBounds(int32_t number);
uint32_t GetCurrentIndex(uint32_t i, uint32_t j, uint32_t k, int32_t size_y, int32_t size_z);

class WrapperElement {
public:
    WrapperElement(uint16_t* pointer_to_main, uint8_t* pointer_to_remainder, uint16_t cur_bit);
    WrapperElement& operator=(int32_t number);
    friend std::ostream& operator<<(std::ostream& stream, const WrapperElement& element);
    friend std::istream& operator>>(std::istream& stream, WrapperElement& ceil);
    operator int() const;
private:
    int MakeInt() const;

    uint16_t* pointer_to_main_;
    uint8_t* pointer_to_remainder_;
    uint16_t bit_index_;
};


class Wrapper1D {
public:
    Wrapper1D(uint32_t index_i, uint32_t index_j, int32_t y_size, int32_t z_size, uint16_t* one_d_array, uint8_t* remainder_array);
    WrapperElement operator[](uint32_t index);
private:
    uint32_t i_;
    uint32_t j_;
    int32_t y_size_;
    int32_t z_size_;
    uint16_t* pointer_to_main_;
    uint8_t* pointer_to_remainder_;
};


class Wrapper2D {
public:
    Wrapper2D(uint32_t index, int32_t y_size, int32_t z_size, uint16_t* two_d_array, uint8_t* remainder_array);
    Wrapper1D operator[](uint32_t index);
private:
    uint32_t i_;
    int32_t y_size_;
    int32_t z_size_;
    uint16_t* pointer_to_main_;
    uint8_t* pointer_to_remainder_;
};
