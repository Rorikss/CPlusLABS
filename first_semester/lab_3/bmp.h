#include <iostream>
#include <fstream>
#include <cstdint>
#include <cstring>
#include <algorithm>

struct FileHeader {
    uint16_t field = 0x4D42;
    uint32_t file_size;
    uint32_t reserved = 0x00000000;
    uint32_t offset = (74 << 24);

    FileHeader(uint64_t size);
};

class Image {
public:
    Image(uint16_t width, uint16_t height);

    void MakeShift(unsigned char* header, uint16_t index, uint16_t parameter);
    void WriteHeader(uint64_t file_size, std::ofstream& file);
    void Export(const char* path, uint64_t** grid, uint16_t start_y, uint16_t start_x);
    void MakeWhite(unsigned char* data);
    void MakeGreen(unsigned char* data);
    void MakeYellow(unsigned char* data);
    void MakePurple(unsigned char* data);
private:
    uint16_t width_;
    uint16_t height_;
    unsigned char kNoColor = 0;
    unsigned char kBlue = 255;
    unsigned char kRed = 255;
    unsigned char kGreen = 255;
    unsigned char kPaleRed = 150; // for beautiful purple. Yes, I need beautiful purple.
    static const uint16_t kFileHeaderSize = 14;
    static const uint16_t kHeaderDataSize = 60;
    static const uint16_t kMultipleFour = 4;
};
