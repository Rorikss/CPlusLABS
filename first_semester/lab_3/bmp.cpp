#include "bmp.h"
#include <cmath>

FileHeader::FileHeader(uint64_t size) {
    file_size = size;
}

Image::Image(uint16_t width, uint16_t height) {
    width_ = width;
    height_ = height;
}

void Image::MakeShift(unsigned char* header, uint16_t index, uint16_t parameter) {
    header[index] = parameter;
    header[++index] = (parameter >> 8);
    header[++index] = (parameter >> 16);
    header[++index] = (parameter >> 24);
}

void Image::MakeWhite(unsigned char* data) {
    data[40] = kBlue;
    data[41] = kGreen;
    data[42] = kRed;
}

void Image::MakeGreen(unsigned char* data) {
    data[44] = kNoColor;
    data[45] = kGreen;
    data[46] = kNoColor;
}

void Image::MakeYellow(unsigned char* data) {
    data[48] = kNoColor;
    data[49] = kGreen;
    data[50] = kRed;
}

void Image::MakePurple(unsigned char* data) {
    data[52] = kBlue;
    data[53] = kNoColor;
    data[54] = kPaleRed;
}

void Image::WriteHeader(uint64_t file_size, std::ofstream& file) {
    unsigned char file_header[] = {
            0, 0, // BM
            0, 0, 0, 0, // File Size
            0, 0, 0, 0, // Reserved for god knows what
            0, 0, 0, 0, // Start of pixel data (14 + 60)
    };

    file_header[0] = static_cast<unsigned char> ('B');
    file_header[1] = static_cast<unsigned char> ('M');
    MakeShift(file_header, 2, file_size);
    file_header[10] = static_cast<unsigned char> (Image::kFileHeaderSize + Image::kHeaderDataSize);
    FileHeader header = FileHeader(file_size);

    unsigned char header_data[] = {
            0, 0, 0, 0, // Size
            0, 0, 0, 0, // Width
            0, 0, 0, 0, // Height
            0, 0, // Planes
            0, 0, // Bits per pixel
            0, 0, 0, 0, // Compression
            0, 0, 0, 0, // Image Size
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0, // Colours Used
            0, 0, 0, 0, // Important Colors

            0, 0, 0, 0, // Color Index r g b
            0, 0, 0, 0, // Color Index r g b
            0, 0, 0, 0, // Color Index r g b
            0, 0, 0, 0, // Color Index r g b
            0, 0, 0, 0, // Color Index r g b
    };

    header_data[0] = static_cast<unsigned char> (40);

    MakeShift(header_data, 4, width_);
    MakeShift(header_data, 8, height_);

    header_data[12] = static_cast<unsigned char> (1);
    header_data[14] = static_cast<unsigned char> (4); // bits per pixel
    header_data[32] = static_cast<unsigned char> (16); // number of colors

    MakeWhite(header_data);
    MakeGreen(header_data);
    MakeYellow(header_data);
    MakePurple(header_data);

   //file.write(reinterpret_cast<char*>(&header), sizeof(header));
//    file.write(reinterpret_cast<char*>(header.file_size), 32);
//    file.write(reinterpret_cast<char*>(header.reserved), 32);
//    file.write(reinterpret_cast<char*>(header.offset), 32);
    file.write(reinterpret_cast<char*>(file_header), Image::kFileHeaderSize);
    file.write(reinterpret_cast<char*>(header_data), Image::kHeaderDataSize);
}

void Image::Export(const char *path, uint64_t **grid, uint16_t start_y, uint16_t start_x) {
    std::ofstream file(path, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        throw std::invalid_argument("File can't be opened.");
    }

    uint64_t half_of_width = std::ceil(static_cast<float> (width_) / 2);
    uint64_t padding_size = (Image::kMultipleFour - half_of_width % Image::kMultipleFour) % kMultipleFour;
    uint64_t file_size = Image::kFileHeaderSize + Image::kHeaderDataSize + height_ * (half_of_width + padding_size);

    WriteHeader(file_size, file);

    unsigned char padding = 0;
    for (uint16_t y = 0; y < height_; ++y) {
        for (uint16_t x = 0; x < width_; x += 2) {
            unsigned char color = 0;
            unsigned char color_first;
            unsigned char color_second;
            color_first = static_cast<char>(grid[start_y + (height_ - y - 1)][start_x + x]);
            if (x + 1 < width_) {
                color_second = static_cast<char>(grid[start_y + (height_ - y - 1)][start_x + x + 1]);
            } else {
                color_second = static_cast<char>(0);
            }
            color = (color_first << 4) | color_second;
            file.write(reinterpret_cast<char*>(&color), 1);
        }
        file.write(reinterpret_cast<char*>(&padding), padding_size);
    }
}
