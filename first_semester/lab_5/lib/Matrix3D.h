#pragma once
#include <iostream>
#include <cstdint>
#include <cmath>
#include "Wrappers.h"

class Matrix3D {
public:
    Matrix3D(int32_t x, int32_t y, int32_t z);
    ~Matrix3D();
    Matrix3D(const Matrix3D& other);
    Matrix3D& operator=(const Matrix3D& other);
    static Matrix3D make_array(int32_t x, int32_t y, int32_t z);

    Wrapper2D operator[](int32_t i);
    friend std::ostream& operator<<(std::ostream& stream, Matrix3D& array);
    friend std::istream& operator>>(std::istream& stream, Matrix3D& array);

    Matrix3D operator*(int32_t lambda);
    Matrix3D operator+(const Matrix3D& other);
    Matrix3D operator-(const Matrix3D& other);

private:
    template <typename T>
    void Swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
    void SwapValues(Matrix3D& first, Matrix3D& second) noexcept;
    int MakeIntNumber(uint32_t i, uint32_t j, uint32_t k) const;
    void Write17BitNumber(uint32_t number, uint32_t i, uint32_t j, uint32_t k);

    uint16_t* main_part_of_number_;
    uint8_t* remainder_part_of_number_;
    int32_t x_size_ = 0;
    int32_t y_size_ = 0;
    int32_t z_size_ = 0;
    int32_t full_size_ = 0;
    int32_t full_remainder_size_ = 0;
    uint8_t kByteSize = 8;
};
