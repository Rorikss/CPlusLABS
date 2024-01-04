#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <algorithm>

enum DirectExpansion {
    up,
    down,
    left,
    right
};

#pragma pack(push, 1)
struct SandPile {
    std::string input_file;
    std::string output_directory;
    uint32_t frequency;
    uint64_t max_iter;
    bool is_stable = false;

    //grid and some specs
    uint16_t width = 1;
    uint16_t height = 1;
    uint16_t real_width = 1;
    uint16_t real_height = 1;
    uint16_t start_x = 0;
    uint16_t start_y = 0;
    uint16_t old_start_y;
    uint16_t old_start_x;
    uint16_t old_height;
    uint16_t old_width;
    uint64_t** grid;

    //some constants
    const int16_t kZero = 0;
    const uint16_t kExpansionMultiplier = 2;
    const uint8_t kMaximalGrainQuantity = 4;

    // functions
    SandPile(std::string input, const std::string output, uint64_t freq, uint64_t mx_iter);
    void EnlargeGrid(DirectExpansion direction);
    void InitializeGrid();
    void EvaluateFromTSV();
    void ScatterSand();
    void ScatterUp(uint16_t y, uint16_t x);
    void ScatterDown(uint16_t y, uint16_t x);
    void ScatterLeft(uint16_t y, uint16_t x);
    void ScatterRight(uint16_t y, uint16_t x);
    void CreateImage(uint64_t iter);
};
#pragma pack(pop)
