//
// Created by Roriks on 5/3/2024.
//

#include "../lib/BST.h"
#include <iostream>
#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace BST;

template <typename T>
struct BSTTestParams {
    std::vector<T> input_values;
    T special_value;
    std::string expexted_output;
};

TEST(BSTTestSuite, IteratorBeginTest) {
    BinarySearchTree<int, PreOrderTag> tree;
    tree.insert(4);
    tree.insert(5);
    auto begin = tree.begin();
    ASSERT_EQ(begin->value, 4);
}


TEST(BSTTestSuite, SizeTest) {
    BinarySearchTree<char, PreOrderTag> tree = {'a', 'b', 'c'};
    ASSERT_EQ(tree.size(), 3);
}

TEST(BSTTestSuite, FindValidTest) {
    BinarySearchTree<int, InOrderTag> tree = {1, 2, 3, 4, 5, 6};
    auto find = tree.find(5);
    ASSERT_EQ(find->value, 5);
}

TEST(BSTTestSuite, FindTraverseTest) {
    BinarySearchTree<int, InOrderTag> tree = {5, 7, 6, 8, 3, 4};
    auto it = tree.find(6);
    std::stringstream output;
    while (it != tree.end()) {
        output << it->value;
        it++;
    }
    std::string expected_output = "678";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, FindInvalidTest) {
    BinarySearchTree<int, InOrderTag> tree = {1, 2, 3, 4, 5, 6};
    auto find = tree.find(10);
    ASSERT_EQ(find, tree.end());
}

TEST(BSTTestSuite, EmptyTest) {
    BinarySearchTree<int, InOrderTag> tree;
    ASSERT_EQ(tree.empty(), true);
}

TEST(BSTTestSuite, InvalidTagTest) {
    struct MyStruct {};
    try {
        BinarySearchTree<int, MyStruct> tree;
    } catch (const std::runtime_error& e) {
        std::cout << "error!!";
    }
}

TEST(BSTTestSuite, InOrderTraverseIntTest) {
    BinarySearchTree<int, InOrderTag> tree = {5, 2, 3, 4, 1};
    std::stringstream output;
    auto it = tree.begin();
    for (size_t i = 0; i < tree.size(); ++i) {
        output << (*it); // also tests *
        ++it; // also checks increment
    }
    std::string expected_output = "12345";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, PreOrderTraverseIntTest) {
    BinarySearchTree<int, PreOrderTag> tree = {5, 3, 8, 4, 2, 7, 9};
    std::stringstream output;
    auto it = tree.begin();
    for (size_t i = 0; i < tree.size(); ++i) {
        output << (*it); // also tests *
        ++it; // also checks increment
    }
    std::string expected_output = "5324879";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, PostOrderTraverseIntTest) {
    BinarySearchTree<int, PostOrderTag> tree = {5, 3, 8};
    std::stringstream output;
    auto it = tree.begin();
    for (size_t i = 0; i < tree.size(); ++i) {
        output << (*it); // also tests *
        ++it; // also checks increment
    }
    std::string expected_output = "385";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, AutoTraverseIntTest) {
    BinarySearchTree<int, InOrderTag> tree = {10, 8, 6, 4, 2, 1, 3, 5, 7, 9};
    std::stringstream output;
    for (auto& item : tree) {
        output << item;
    }
    std::string expected_output = "12345678910";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, ReverseInOrderTraverseIntTest) {
    BinarySearchTree<int, InOrderTag> tree = {5, 2, 3, 4, 1};
    std::stringstream output;
    auto it = tree.end();
    it--;
    while (it != tree.begin()) {
        output << it->value;
        --it;
    }
    output << it->value;
    std::string expected_output = "54321";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, ReversePreOrderTraverseIntTest) {
    BinarySearchTree<int, PreOrderTag> tree = {5, 7, 6, 8, 3, 4};
    std::stringstream output;
    for (auto i = tree.end(); i != tree.begin(); i--) {
        output << i->value;
    }
    std::string expected_output = "086743";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, ReversePostOrderTraverseIntTest) {
    BinarySearchTree<int, PostOrderTag> tree = {5, 7, 6, 8, 3, 4};
    std::stringstream output;
    auto it = tree.end();
    for (int i = 0; i < tree.size(); ++i) {
        output << it->value;
        it--;
    }
    std::string expected_output = "057863";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, InsertCharTest) {
    BinarySearchTree<char, InOrderTag> tree;
    tree.insert('c');
    tree.insert('b');
    tree.insert('a');
    tree.insert('f');
    tree.insert('d');
    tree.insert('e');
    std::stringstream output;
    for (auto& item : tree) {
        output << item;
    }
    std::string expected_output = "abcdef";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, InsertStringAndTraverseTest) {
    BinarySearchTree<std::string, InOrderTag> tree;
    tree.insert("apple");
    tree.insert("octopus");
    tree.insert("cat");
    std::stringstream output;
    for (auto& item : tree) {
        output << item;
    }
    std::string expected_output = "applecatoctopus";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, InsertIntTest) {
    BinarySearchTree<int, PreOrderTag> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(8);
    tree.insert(7);
    tree.insert(9);
    std::stringstream output;
    auto it = tree.begin();
    for (size_t i = 0; i < tree.size(); ++i) {
        output << it->value;
        ++it;
    }
    std::string expected_output = "53879";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, ExtractCharTest) {
    BinarySearchTree<char, InOrderTag> tree = {'z', 'a', 't', 'b', 'c'};
    tree.extract('t');
    std::stringstream output;
    for (auto& item : tree) {
        output << item;
    }
    std::string expected_output = "abcz";
    ASSERT_EQ(output.str(), expected_output);
}

class BSTExtractIntTest : public testing::TestWithParam<BSTTestParams<int>> {
};

TEST_P(BSTExtractIntTest, ExtractTest) {
    BinarySearchTree<int, InOrderTag> tree;
    for (int value : GetParam().input_values) {
        tree.insert(value);
    }
    tree.extract(GetParam().special_value);
    std::stringstream output;
    for (auto& item : tree) {
        output << item;
    }
    ASSERT_EQ(output.str(), GetParam().expexted_output);
}

INSTANTIATE_TEST_SUITE_P(
    ExtractIntTestCases,
    BSTExtractIntTest,
    testing::Values(
            BSTTestParams<int>{{5, 2, 3, 4, 1}, 3, "1245"},
            BSTTestParams<int>{{5, 2, 3, 4, 1}, 4, "1235"},
            BSTTestParams<int>{{5, 2, 3, 4, 1}, 1, "2345"},
            BSTTestParams<int>{{28, 45, 67, 91, 9, 13}, 45, "913286791"},
            BSTTestParams<int>{{5, 2, 3, 4, 1}, 18, "12345"}
    ));

TEST(BSTTestSuite, ContainsTrueTest) {
    BinarySearchTree<int, PostOrderTag> tree = {1, 2, 3, 4, 5};
    ASSERT_EQ(tree.contains(3), true);
}

TEST(BSTTestSuite, ContainsFalseTest) {
    BinarySearchTree<int, PostOrderTag> tree = {1, 2, 3, 4, 5};
    ASSERT_EQ(tree.contains(33), false);
}

TEST(BSTTestSuite, LowerBoundTest) {
    BinarySearchTree<int, PostOrderTag> tree = {7, 2, 10, 9, 12, 1, 5};
    ASSERT_EQ(tree.lower_bound(2)->value, 1);
}

TEST(BSTTestSuite, UpperBoundTest) {
    BinarySearchTree<int, PostOrderTag> tree = {7, 2, 10, 9, 12, 1, 5};
    ASSERT_EQ(tree.upper_bound(2)->value, 5);
}

TEST(BSTTestSuite, EmplaceTest) {
    BinarySearchTree<int, InOrderTag> tree;
    tree.emplace(5);
    tree.emplace(3);
    tree.emplace(8);
    std::stringstream output;
    for (auto& item : tree) {
        output << item;
    }
    std::string expected_output = "358";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, InsertRangeTest) {
    BinarySearchTree<int, InOrderTag> tree;
    tree.insert(1, 6);
    std::stringstream output;
    for (auto& item : tree) {
        output << item;
    }
    std::string expected_output = "12345";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, EqualtyTest) {
    BinarySearchTree<long long, InOrderTag> tree_1 = {100, 78, 100500500};
    BinarySearchTree<long long, InOrderTag> tree_2 = {100, 78, 100500500};
    ASSERT_EQ(tree_1 == tree_2, true);
}

TEST(BSTTestSuite, UnequaltyTest) {
    BinarySearchTree<long long, InOrderTag> tree_1 = {100, 78, 100500500};
    BinarySearchTree<long long, InOrderTag> tree_2 = {6798, 37542828, 100500500};
    ASSERT_EQ(tree_1 != tree_2, true);
}

TEST(BSTTestSuite, AssignmentTest) {
    BinarySearchTree<int, InOrderTag> tree = {5, 7, 6, 8, 3, 4, 1};
    BinarySearchTree<int, InOrderTag> tree_1 = tree;
    ASSERT_EQ(tree_1 == tree, true);
}

TEST(BSTTestSuite, AssignmentIndependenceTest) {
    BinarySearchTree<int, InOrderTag> tree = {5, 7, 6, 8, 3, 4};
    BinarySearchTree<int, InOrderTag> tree_1 = tree;
    tree_1.insert(1);
    std::stringstream output;
    for (auto& x : tree_1) {
        output << x;
    }
    for (auto& x : tree) {
        output << x;
    }
    std::string expected_output = "1345678345678";
    ASSERT_EQ(output.str(), expected_output);
}

TEST(BSTTestSuite, CopyTest) {
    BinarySearchTree<int, InOrderTag> tree = {5, 7, 6, 8, 3, 4, 1};
    BinarySearchTree<int, InOrderTag> tree_1(tree);
    ASSERT_EQ(tree_1 == tree, true);
}

TEST(BSTTestSuite, MergeTest) {
    BinarySearchTree<int, InOrderTag> tree = {1, 2};
    BinarySearchTree<int, InOrderTag> tree_1 = {3, 4};
    tree.merge(tree_1);
    BinarySearchTree<int, InOrderTag> ans = {1, 2, 3, 4};
    ASSERT_EQ(ans == tree, true);
}

TEST(BSTTestSuite, CountTest) {
    BinarySearchTree<int, PostOrderTag> tree = {5, 2, 2, 3, 4};
    ASSERT_EQ(tree.count(2), 2);
}

TEST(BSTTestSuite, ReverseIteratorTest) {
    BinarySearchTree<int, InOrderTag> tree = {5, 3, 4};
    auto it = tree.rend();
    ASSERT_EQ(it.base()->value, 3);
}
