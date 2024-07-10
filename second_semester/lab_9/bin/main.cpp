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
    std::unordered_map<int, int> map = {{1, 2}, {3, 4}, {4, 5}, {5, 6}};
    std::set<int> aboba = {1, 2, 3, 4, 5};
    auto ptr = aboba.begin();
    
    return 0;
}
