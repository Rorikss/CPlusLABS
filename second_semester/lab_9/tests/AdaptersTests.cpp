//
// Created by Roriks on 3/22/2024.
//
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include "gtest/gtest.h"
#include <gtest/gtest-spi.h>
#include "../lib/Adapters.h"

using namespace adapters;

TEST(AdaptersTestSuite, TransformVectorDefaultTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto transformed = v | transform([](int i){return i * i;});
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "149162536");
}

TEST(AdaptersTestSuite, FilterVectorDefaultTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto filtered = v | filter([](int i){ return i % 2 == 0;});
    std::stringstream output;
    for (auto x : filtered) {
        output << x;
    }
    ASSERT_EQ(output.str(), "246");
}

TEST(AdaptersTestSuite, TakeVectorDefaultTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto transformed = v | take(3);
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "123");
}

TEST(AdaptersTestSuite, DropVectorDefaultTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto transformed = v | drop(3);
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "456");
}

TEST(AdaptersTestSuite, ReverseVectorDefaultTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto reversed = v | reverse();
    std::stringstream output;
    for (auto x : reversed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "654321");
}

TEST(AdaptersTestSuite, DoubleReverseVectorTest) {
    std::vector<char> v = {'a', 'b', 'c', 'd'};
    auto transformed = v | reverse() | reverse();
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "abcd");
}

TEST(AdaptersTestSuite, KeysMapDefaultTest) {
    std::map<int, int> mapa = {{1, 2}, {3, 4}, {5, 6}};
    auto transformed = mapa | keys();
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "135");
}

TEST(AdaptersTestSuite, ValuesMapDefaultTest) {
    std::map<int, int> mapa = {{1, 2}, {3, 4}, {5, 6}};
    auto transformed = mapa | values();
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "246");
}

TEST(AdaptersTestSuite, ValuesSetDefaultTest) {
    std::set<int> s = {1, 2, 3, 4, 5, 6};
    auto transformed = s | values();
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "123456");
}

TEST(AdaptersTestSuite, SetFilterStringTest) {
    std::set<std::string> s = {"hello", "goodbye", "yes", "no"};
    std::stringstream output;
    for (auto x : s | filter([] (std::string str) {return str.length() > 4; })) {
        output << x;
    }
    ASSERT_EQ(output.str(), "goodbyehello");
}

TEST(AdaptersTestSuite, RvalueCombinationTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    std::stringstream output;
    for (auto x : v | transform([](int i){return 3 * i;})
        | filter([](int i){ return i % 2 == 0;}) | take(6) | drop(3)) {
        output << x;
    }
    ASSERT_EQ(output.str(), "243036");
}

TEST(AdaptersTestSuite, CombinationMapedTest) {
    std::map<int, int> mapa = {{1, 2}, {3, 3}, {5, 11}, {7, 15}, {9, 10}};
    std::stringstream output;
    for (auto x : mapa | values() | transform([](int i){return i * i;})
        | filter([](int i){ return i % 2 == 0;}) | reverse()) {
        output << x;
    }
    ASSERT_EQ(output.str(), "1004");
}

TEST(AdaptersTestSuite, TransformStringVectorTest) {
    std::vector<std::string> v = {"ab", "cat", "dog", "apple"};
    auto transformed = v | transform([](std::string i){return i.length();});
    std::stringstream output;
    for (auto x : transformed) {
        output << x;
    }
    ASSERT_EQ(output.str(), "2335");
}

TEST(AdaptersTestSuite, FilterListTest) {
    std::list<int> v = {28, 90, 34, 18, 56, 81, 23, 31, 88};
    std::stringstream output;
    for (auto x : v | filter([](int i){ return i % 10 == 8;})) {
        output << x;
    }
    ASSERT_EQ(output.str(), "281888");
}

TEST(AdaptersTestSuite, IteratorDefaultTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto transformed = v | transform([](int i){return i * i;});
    auto it = transformed.begin();
    ASSERT_EQ(*it, 1);
}

TEST(AdaptersTestSuite, IteratorIncrementAndReversedTest) {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto reversed = v | reverse();
    auto it = reversed.begin();
    ++it;
    ASSERT_EQ(*it, 5);
}

TEST(AdaptersTestSuite, IteratorMapTest) {
    std::map<int, int> mapa = {{1, 2}, {3, 4}, {5, 6}};
    auto vals = mapa | values();
    auto it = vals.begin();
    ASSERT_EQ(*it, 2);
}

