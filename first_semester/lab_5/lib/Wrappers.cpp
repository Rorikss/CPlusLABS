#include "Wrappers.h"

uint8_t kMinimalValue = 0;
uint32_t kMaximalValue = 1 << 17;
uint8_t kByteSize = 8;
void CheckNumberIsInBounds(int32_t number) {
    if (number < kMinimalValue) {
        throw std::runtime_error("Only positive numbers may be entered");
    }
    if (number > kMaximalValue) {
        throw std::runtime_error("Too big value, Matrix3D store only 17bit positive numbers");
    }
}

uint32_t GetCurrentIndex(uint32_t i, uint32_t j, uint32_t k, int32_t size_y, int32_t size_z) {
    return (i * size_y * size_z + j * size_z + k);
}

// Wrapper elem methods
WrapperElement::WrapperElement(uint16_t* pointer_to_main, uint8_t* pointer_to_remainder, uint16_t cur_bit)
        : pointer_to_main_(pointer_to_main),
          pointer_to_remainder_(pointer_to_remainder),
          bit_index_(cur_bit)
        {}

WrapperElement& WrapperElement::operator=(int32_t number) {
    CheckNumberIsInBounds(number);
    uint16_t new_main_part = (number >> 1);
    *pointer_to_main_ = new_main_part;
    uint8_t carry = (number & 1) << (bit_index_);
    uint8_t new_remainder = *pointer_to_remainder_;
    new_remainder &= ~(1 << bit_index_);
    new_remainder |= carry;
    *pointer_to_remainder_ = new_remainder;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const WrapperElement& element) {
    return stream << element.MakeInt();
}

std::istream& operator>>(std::istream& stream, WrapperElement& ceil) {
    int number;
    stream >> number;
    CheckNumberIsInBounds(number);
    if (stream) {
        uint8_t carry = (number & 1) << (ceil.bit_index_);
        uint8_t new_remainder = *ceil.pointer_to_remainder_ | carry;
        *ceil.pointer_to_remainder_ = new_remainder;
        uint16_t new_main_value = (number >> 1);
        *ceil.pointer_to_main_ = new_main_value;
    }
    return stream;
}

WrapperElement::operator int() const {
    return MakeInt();
}

int WrapperElement::MakeInt() const {
    int number = *pointer_to_main_;
    uint8_t carry = (*pointer_to_remainder_ >> bit_index_) & 1;
    number = (number << 1) | carry;
    return number;
}

// Wrapper 1D methods
Wrapper1D::Wrapper1D(uint32_t index_i, uint32_t index_j, int32_t y_size, int32_t z_size, uint16_t* one_d_array, uint8_t* remainder_array)
        : i_(index_i),
          j_(index_j),
          y_size_(y_size),
          z_size_(z_size),
          pointer_to_main_(one_d_array),
          pointer_to_remainder_(remainder_array)
        {}

WrapperElement Wrapper1D::operator[](uint32_t index) {
    if (index >= z_size_) {
        throw std::runtime_error("Index out of range");
    }
    uint32_t cur_index = GetCurrentIndex(i_, j_, index, y_size_, z_size_);
    uint16_t cur_byte = cur_index / kByteSize;
    uint16_t cur_bit = cur_index % kByteSize;
    return WrapperElement(&pointer_to_main_[cur_index], &pointer_to_remainder_[cur_byte], cur_bit);
}

// Wrapper 2D methods
Wrapper2D::Wrapper2D(uint32_t index, int32_t y_size, int32_t z_size, uint16_t* two_d_array, uint8_t* remainder_array)
        : i_(index),
          y_size_(y_size),
          z_size_(z_size),
          pointer_to_main_(two_d_array),
          pointer_to_remainder_(remainder_array)
        {}

Wrapper1D Wrapper2D::operator[](uint32_t index) {
    if (index >= y_size_) {
        throw std::runtime_error("Index out of range");
    }
    return Wrapper1D(i_, index, y_size_, z_size_, pointer_to_main_, pointer_to_remainder_);
}
