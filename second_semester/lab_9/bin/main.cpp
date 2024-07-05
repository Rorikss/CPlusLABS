//
// Created by Roriks on 3/20/2024.
//
#include "./lib/Adapters.h"
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

using namespace adapters;


int main() {
    std::map<int, int> map = {{1, 2}, {3, 4}, {4, 5}, {5, 6}};

    auto removed = map | keys();
    auto it = removed.begin();
    it++;

    for (auto i : removed) {
        std::cout << i << " ";
    }

    std::map<int, int> mapa = {{1, 2}, {3, 3}, {5, 11}, {7, 15}, {9, 10}};
    std::vector<int> v = {1, 2, 3};
    for (auto x : v | transform([](int i){return i * i;}) | reverse()) {

    }
    for (auto x : mapa | values() | transform([](int i){return i * i;})
                  | filter([](int i){ return i % 2 == 0;}) | reverse()) {
        std::cout << x;
    }
    return 0;
}
