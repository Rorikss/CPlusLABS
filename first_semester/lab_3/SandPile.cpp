#include "bmp.h"
#include "SandPile.h"
#include "Parser.h"

SandPile::SandPile(std::string input, std::string output, uint64_t freq, uint64_t mx_iter) {
    input_file = input;
    output_directory = output;
    frequency = freq;
    max_iter = mx_iter;
    EvaluateFromTSV();
    ScatterSand();
}

void SandPile::EnlargeGrid(DirectExpansion direction) {
    uint16_t copy_start_y;
    uint16_t copy_start_x;
    copy_start_y = start_y;
    copy_start_x = start_x;
    switch (direction) {
        case up:
            real_height *= kExpansionMultiplier;
            start_y = real_height - height;
            break;
        case down:
            real_height *= kExpansionMultiplier;
            break;
        case left:
            real_width *= kExpansionMultiplier;
            start_x = real_width - width;
            break;
        case right:
            real_width *= kExpansionMultiplier;
            break;
    }
    uint64_t** tmp { new uint64_t* [real_height] };
    for (uint16_t count { 0 }; count < real_height; ++count) {
        tmp[count] = new uint64_t[real_width] {};
    }
    // copy
    for (uint16_t i = 0; i < height; i++)  {
        for (uint16_t j = 0; j < width; j++) {
            tmp[start_y + i][start_x + j] = grid[copy_start_y + i][copy_start_x + j];
        }
    }
    std::swap(tmp, grid);
    for (uint16_t count { 0 }; count < height; ++count)
        delete[] tmp[count];
    delete[] tmp;
}

void SandPile::InitializeGrid() {
    grid = { new uint64_t* [real_height] };
    for (uint16_t count {0}; count < real_height; ++count) {
        grid[count] = new uint64_t[real_width]{};
    }
}

void SandPile::EvaluateFromTSV() {
    std::ifstream file(input_file);
    if (!file.is_open()) {
        throw std::invalid_argument("File can't be opened.");
    }
    int16_t x;
    int16_t y;
    int16_t minimal_x = std::numeric_limits<int16_t>::max();
    int16_t minimal_y = std::numeric_limits<int16_t>::max();
    int16_t maximal_x = std::numeric_limits<int16_t>::min();
    int16_t maximal_y = std::numeric_limits<int16_t>::min();
    uint64_t grains_quantity;
    std::string str;
    while (file >> x >> y >> grains_quantity) {
        minimal_x = std::min(minimal_x, x);
        minimal_y = std::min(minimal_y, y);
        maximal_x = std::max(maximal_x, x);
        maximal_y = std::max(maximal_y, y);
    }
    real_width = width = maximal_x - minimal_x + 1;
    real_height = height = maximal_y - minimal_y + 1;
    int16_t shift_x = kZero - minimal_x;
    int16_t shift_y = kZero - minimal_y;
    InitializeGrid();
    file.clear();
    file.seekg(0);
    while (file >> x >> y >> grains_quantity) {
        grid[y + shift_y][x + shift_x] = grains_quantity;
    }
}

void SandPile::ScatterUp(uint16_t y, uint16_t x) {
    if (y == 0) {
        if (start_y == 0) {
            EnlargeGrid(up);
        }
        height++;
        old_start_y = start_y;
        start_y--;
        grid[start_y][start_x + x]++;
    } else {
        grid[start_y + y - 1][start_x + x]++;
    }
}

void SandPile::ScatterDown(uint16_t y, uint16_t x) {
    if (y + 1 >= old_height) {
        if (start_y + height == real_height) {
            EnlargeGrid(down);
        }
        grid[start_y + height][old_start_x + x]++;
        height++;
    } else {
        grid[old_start_y + y + 1][old_start_x + x]++;
    }
}

void SandPile::ScatterLeft(uint16_t y, uint16_t x) {
    if (x == 0) {
        if (start_x == 0) {
            EnlargeGrid(left);
        }
        width++;
        old_start_x = start_x;
        start_x--;
        grid[old_start_y + y][start_x]++;
    } else {
        grid[old_start_y + y][start_x + x - 1]++;
    }
}

void SandPile::ScatterRight(uint16_t y, uint16_t x) {
    if (x + 1 >= old_width) {
        if (start_x + width == real_width) {
            EnlargeGrid(right);
        }
        grid[old_start_y + y][old_start_x + old_width]++;
        width++;
    } else {
        grid[old_start_y + y][old_start_x + x + 1]++;
    }
}

void SandPile::ScatterSand(){
    uint64_t iter = 0;
    while (!is_stable && (iter < max_iter || max_iter == 0)) {
        is_stable = true;
        for (uint16_t y = 0; y < height; ++y) {
            for (uint16_t x = 0; x < width; ++x) {
                if (grid[start_y + y][start_x + x] < kMaximalGrainQuantity) {
                    continue;
                }
                old_start_y = start_y;
                old_start_x = start_x;
                old_height = height;
                old_width = width;
                is_stable = false;
                grid[start_y + y][start_x + x] -= kMaximalGrainQuantity;

                ScatterUp(y, x);
                ScatterLeft(y, x);
                ScatterRight(y, x);
                ScatterDown(y, x);
            }
        }
        if (frequency != 0 && iter != 0 && iter % frequency == 0) {
            CreateImage(iter);
        }
        ++iter;
    }
    if (frequency == 0) CreateImage(-1);
}

void SandPile::CreateImage(uint64_t iter) {
    Image image(width, height);
    std::string output_path = output_directory + "sand_pile_image";
    if (iter != -1) {
        output_path += "_" + std::to_string(iter);
    }
    output_path += ".bmp";
    image.Export(static_cast<char*>(output_path.data()), grid, start_y, start_x);
}

