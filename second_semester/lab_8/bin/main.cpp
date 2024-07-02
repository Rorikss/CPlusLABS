//
// Created by avant on 2/28/2024.
//
#include <iostream>
#include <algorithm>
#include "../lib/BST.h"
#include <memory>
#include <numeric>

using namespace BST;


int main() {
    BinarySearchTree<int, InOrderTag> tree = {5, 7, 6, 8, 3, 4};
    int a = std::accumulate(tree.begin(), tree.end(), 0);
    std::cout << a;
}