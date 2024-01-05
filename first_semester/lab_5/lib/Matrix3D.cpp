#include "Matrix3D.h"

// public
Matrix3D::Matrix3D(int32_t x, int32_t y, int32_t z) {
    x_size_ = x;
    y_size_ = y;
    z_size_ = z;
    full_size_ = x * y * z;
    full_remainder_size_ = ceil(static_cast<float>(full_size_) / kByteSize);
    remainder_part_of_number_ = new uint8_t[full_remainder_size_]{};
    main_part_of_number_ = new uint16_t[full_size_]{};
}

Matrix3D::~Matrix3D() {
    delete[] main_part_of_number_;
    delete[] remainder_part_of_number_;
}

Matrix3D::Matrix3D(const Matrix3D& other) {
    x_size_ = other.x_size_;
    y_size_ = other.y_size_;
    z_size_ = other.z_size_;
    full_size_ = other.full_size_;
    full_remainder_size_ = other.full_remainder_size_;
    remainder_part_of_number_ = new uint8_t[full_remainder_size_];
    main_part_of_number_ = new uint16_t[full_size_]{};
    for (uint32_t i = 0; i < full_size_; ++i) {
        main_part_of_number_[i] = other.main_part_of_number_[i];
    }
    for (uint32_t i = 0; i < other.full_remainder_size_; ++i) {
        remainder_part_of_number_[i] = other.remainder_part_of_number_[i];
    }
}

Matrix3D& Matrix3D::operator=(const Matrix3D& other) {
    if (this == &other) {
        return *this;
    }
    Matrix3D tmp(other);
    SwapValues(*this, tmp);
    return *this;
}

Matrix3D Matrix3D::make_array(int32_t x, int32_t y, int32_t z)  {
    return Matrix3D(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(z));
}

Wrapper2D Matrix3D::operator[](int32_t i) {
    if (i >= x_size_) {
        throw std::runtime_error("Index out of range");
    }
    return Wrapper2D(i, y_size_, z_size_, main_part_of_number_, remainder_part_of_number_);
}

std::istream& operator>>(std::istream& stream, Matrix3D& array) {
    for (uint32_t i = 0; i < array.x_size_; ++i) {
        for (uint32_t j = 0; j < array.y_size_; ++j) {
            for (uint32_t k = 0; k < array.z_size_; ++k) {
                auto elem = array[i][j][k];
                stream >> elem;
            }
        }
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, Matrix3D& array) {
    for (uint32_t i = 0; i < array.x_size_; ++i) {
        for (uint32_t j = 0; j < array.y_size_; ++j) {
            for (uint32_t k = 0; k < array.z_size_; ++k) {
                stream << array[i][j][k] << " ";
            }
            stream << "\n";
        }
        stream << "\n";
    }
    return stream;
}

Matrix3D Matrix3D::operator*(int32_t lambda) {
    if (lambda < 0) {
        throw std::runtime_error("Matrix3D stores only positive uint17");
    }
    Matrix3D product = make_array(x_size_, y_size_, z_size_);
    for (uint32_t i = 0; i < x_size_; ++i) {
        for (uint32_t j = 0; j < y_size_; ++j) {
            for (uint32_t k = 0; k < z_size_; ++k) {
                uint32_t number = MakeIntNumber(i, j, k) * lambda;
                product.Write17BitNumber(number, i, j, k);
            }
        }
    }
    return product;
}

Matrix3D Matrix3D::operator+(const Matrix3D& other) {
    if (x_size_ != other.x_size_ || y_size_ != other.y_size_ || z_size_ != other.z_size_) {
        throw std::runtime_error("Invalid matrix size.");
    }
    Matrix3D summary = make_array(x_size_, y_size_, z_size_);
    for (uint32_t i = 0; i < x_size_; ++i) {
        for (uint32_t j = 0; j < y_size_; ++j) {
            for (uint32_t k = 0; k < z_size_; ++k) {
                uint32_t number = MakeIntNumber(i, j, k) + other.MakeIntNumber(i, j, k);
                summary.Write17BitNumber(number, i, j, k);
            }
        }
    }
    return summary;
}

Matrix3D Matrix3D::operator-(const Matrix3D& other) {
    if (x_size_ != other.x_size_ || y_size_ != other.y_size_ || z_size_ != other.z_size_) {
        throw std::runtime_error("Invalid matrix size.");
    }
    Matrix3D summary = make_array(x_size_, y_size_, z_size_);
    for (uint32_t i = 0; i < x_size_; ++i) {
        for (uint32_t j = 0; j < y_size_; ++j) {
            for (uint32_t k = 0; k < z_size_; ++k) {
                int32_t lhs = MakeIntNumber(i, j, k);
                int32_t rhs = other.MakeIntNumber(i, j, k);
                uint32_t number = lhs - rhs;
                summary.Write17BitNumber(number, i, j, k);
            }
        }
    }
    return summary;
}

//privates
void Matrix3D::SwapValues(Matrix3D& first, Matrix3D& second) noexcept {
    Swap(first.x_size_, second.x_size_);
    Swap(first.y_size_, second.y_size_);
    Swap(first.z_size_, second.z_size_);
    Swap(first.full_size_, second.full_size_);
    Swap(first.remainder_part_of_number_, second.remainder_part_of_number_);
    Swap(first.main_part_of_number_, second.main_part_of_number_);
}

int Matrix3D::MakeIntNumber(uint32_t i, uint32_t j, uint32_t k) const {
    uint32_t cur_index = GetCurrentIndex(i, j, k, y_size_, z_size_);
    uint16_t cur_byte = cur_index / kByteSize;
    uint16_t cur_bit = cur_index % kByteSize;
    uint32_t number = main_part_of_number_[cur_index];
    int carry = (remainder_part_of_number_[cur_byte] >> cur_bit) & 1;
    number = (number << 1) | carry;
    return number;
}

void Matrix3D::Write17BitNumber(uint32_t number, uint32_t i, uint32_t j, uint32_t k) {
    CheckNumberIsInBounds(number);
    uint32_t cur_index = GetCurrentIndex(i, j, k, y_size_, z_size_);
    uint16_t cur_byte = cur_index / kByteSize;
    uint16_t cur_bit = cur_index % kByteSize;
    uint8_t carry = number & (1 << 0);
    remainder_part_of_number_[cur_byte] &= ~(1 << cur_bit);
    remainder_part_of_number_[cur_byte] |= (carry << (cur_bit));
    main_part_of_number_[cur_index] = (number >> 1);
}
