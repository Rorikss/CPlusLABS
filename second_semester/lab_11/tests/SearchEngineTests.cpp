#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include <bitset>
#include "../lib/Indexer.h"
#include "../lib/Searcher.h"

using namespace SearchEngine;

TEST(VarIntTestSuite, DefaultSIZETTest) {
    VarInt<size_t> var_int;
    size_t num = 1234;
    std::vector<size_t> a = {1, 54, 28, 128, 85923476, 123586, 65345, 124, 1234567890};
    auto compressed = var_int.Compress(num, a);
    auto decompressed = var_int.Decompress(compressed);
    a.insert(a.begin(), num);
    ASSERT_EQ(a, decompressed);
}

TEST(VarIntTestSuite, HugeSIZETTest) {
    VarInt<size_t> var_int;
    size_t num = 1212345623434;
    std::vector<size_t> a = {12345678390, 98763526482, 12353458283452, 23453845345833451, 12345678234567887};
    auto compressed = var_int.Compress(num, a);
    auto decompressed = var_int.Decompress(compressed);
    a.insert(a.begin(), num);
    ASSERT_EQ(a, decompressed);
}

TEST(VarIntTestSuite, RepeatedValuesSIZETTest) {
    VarInt<size_t> var_int;
    size_t num = 2;
    std::vector<size_t> a = {0, 0, 2, 2, 2, 4, 4, 4, 16, 16, 16, 32, 32, 32, 64, 64};
    auto compressed = var_int.Compress(num, a);

    auto decompressed = var_int.Decompress(compressed);
    a.insert(a.begin(), num);
    ASSERT_EQ(std::vector<size_t>({ 2, 0, 2, 4, 16, 32, 64 }), decompressed);
}

TEST(VarIntTestSuite, EvenToTwoSIZETTest) {
    VarInt<size_t> var_int;
    size_t num = 243445;
    std::vector<size_t> a = {9, 0, 2, 4, 8, 16, 32, 128, 256, 64, 129};
    auto compressed = var_int.Compress(num, a);

    auto decompressed = var_int.Decompress(compressed);
    a.insert(a.begin(), num);
    ASSERT_EQ(a, decompressed);
}

TEST(VarIntTestSuite, DefaultUINTTest) {
    VarInt<uint32_t> var_int;
    uint32_t num = 10;
    std::vector<uint32_t> a = {2, 43, 256, 1024, 36789, 432, 2537};
    auto compressed = var_int.Compress(num, a);
    auto decompressed = var_int.Decompress(compressed);
    a.insert(a.begin(), num);
    ASSERT_EQ(a, decompressed);
}

TEST(VarIntTestSuite, DefaultUINT16Test) {
    VarInt<uint16_t> var_int;
    uint16_t num = 101;
    std::vector<uint16_t> a = {22, 18, 28, 14, 256, 43, 253, 100, 128};
    auto compressed = var_int.Compress(num, a);
    auto decompressed = var_int.Decompress(compressed);
    a.insert(a.begin(), num);
    ASSERT_EQ(a, decompressed);
}

TEST(SyntaxTreeTestSuite, DefaultANDTest) {
    SyntaxParser parser;
    parser.MakeQuery("a AND c");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "c", "AND"}));
}

TEST(SyntaxTreeTestSuite, DefaultORTest) {
    SyntaxParser parser;
    parser.MakeQuery("a OR c");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "c", "OR"}));
}

TEST(SyntaxTreeTestSuite, DefaultWordTest) {
    SyntaxParser parser;
    parser.MakeQuery("aboba");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"aboba"}));
}

TEST(SyntaxTreeTestSuite, OrAndTest) {
    SyntaxParser parser;
    parser.MakeQuery("a OR b AND c");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({ "a", "b", "c", "AND", "OR" }));
}

TEST(SyntaxTreeTestSuite, AndOrTest) {
    SyntaxParser parser;
    parser.MakeQuery("a AND b OR c");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "b", "AND", "c", "OR"}));
}

TEST(SyntaxTreeTestSuite, OrOrTest) {
    SyntaxParser parser;
    parser.MakeQuery("a OR b OR c");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "b", "OR", "c", "OR"}));
}

TEST(SyntaxTreeTestSuite, AndAndTest) {
    SyntaxParser parser;
    parser.MakeQuery("a AND b AND c");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "b", "AND", "c", "AND"}));
}

TEST(SyntaxTreeTestSuite, MyltiBraketTest) {
    SyntaxParser parser;
    parser.MakeQuery("((a))");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a"}));
}

TEST(SyntaxTreeTestSuite, DefaultBraketTest) {
    SyntaxParser parser;
    parser.MakeQuery("(a OR b)");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "b", "OR"}));
}

TEST(SyntaxTreeTestSuite, ReverseBraketTest) {
    SyntaxParser parser;
    parser.MakeQuery("a OR (b AND c)");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({ "a", "b", "c", "AND", "OR" }));
}

TEST(SyntaxTreeTestSuite, PriorityChangeBraketTest) {
    SyntaxParser parser;
    parser.MakeQuery("(a OR b) AND c");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "b", "OR", "c", "AND"}));
}

TEST(SyntaxTreeTestSuite, DoubleBraketTest) {
    SyntaxParser parser;
    parser.MakeQuery("(a OR b) AND (c OR d)");
    auto order = parser.MakeParseOrder();
    ASSERT_EQ(order, std::vector<std::string>({"a", "b", "OR", "c", "d", "OR", "AND"}));
}

TEST(SyntaxTreeTestSuite, InvalidTest) {
    SyntaxParser parser;
    ASSERT_ANY_THROW(parser.MakeQuery("aboba kek lol"));
}

TEST(SyntaxTreeTestSuite, InvalidBraket1Test) {
    SyntaxParser parser;
    ASSERT_ANY_THROW(parser.MakeQuery("(a))"));
}

TEST(SyntaxTreeTestSuite, InvalidBraket2Test) {
    SyntaxParser parser;
    ASSERT_ANY_THROW(parser.MakeQuery("(a AND b"));
}

TEST(SyntaxTreeTestSuite, InvalidOperators1Test) {
    SyntaxParser parser;
    ASSERT_ANY_THROW(parser.MakeQuery("a AND b OR"));
}

TEST(SyntaxTreeTestSuite, InvalidOperators2Test) {
    SyntaxParser parser;
    ASSERT_ANY_THROW(parser.MakeQuery("AND OR a OR b"));
}

TEST(SearcherTestSuite, DefaultSearchTest) {
    std::stringstream index("3 10 aboba.txt 4 aurora.txt 8 misha.txt 11 bebe 2 4 \u0001\u0001\u0001\u0001 2 \u0002  are 2 2 \u0001  2 \u0002\u0002 how 2 2 \u0001  2 \u0002\u0002 hello 1 2 \u0002\u0001 you 2 2 \u0001  2 \u0002\u0002 hi 1 2 \u0001  banana 3 2  \u0002 2 \u0001\u0004 2 \u0002\u0004 apple 2 2  \u0001 2 \u0002\u0003 morning 2 2    2 \u0002\u0005 good 2 2    2 \u0002\u0005 ");
    Searcher search = Searcher("output", 5, 0.75);
    std::stringstream ask("bebe");
    std::stringstream answer;
    search.TestOneQuery(index, ask, answer);
    ASSERT_EQ(answer.str(), "Found your search in files:\n1. misha.txt on lines: 33 \n2. aurora.txt on lines: 2 3 4 \n");
}

TEST(SearcherTestSuite, MultiBktSearchTest) {
    std::stringstream index("3 10 aboba.txt 4 aurora.txt 8 misha.txt 11 bebe 2 4 \u0001\u0001\u0001\u0001 2 \u0002  are 2 2 \u0001  2 \u0002\u0002 how 2 2 \u0001  2 \u0002\u0002 hello 1 2 \u0002\u0001 you 2 2 \u0001  2 \u0002\u0002 hi 1 2 \u0001  banana 3 2  \u0002 2 \u0001\u0004 2 \u0002\u0004 apple 2 2  \u0001 2 \u0002\u0003 morning 2 2    2 \u0002\u0005 good 2 2    2 \u0002\u0005 ");
    Searcher search = Searcher("output", 5, 0.75);
    std::stringstream ask("((hi))");
    std::stringstream answer;
    search.TestOneQuery(index, ask, answer);
    ASSERT_EQ(answer.str(), "Found your search in files:\n1. aurora.txt on lines: 33 \n");
}

TEST(SearcherTestSuite, ANDSearchTest) {
    std::stringstream index("3 10 aboba.txt 4 aurora.txt 8 misha.txt 11 bebe 2 4 \u0001\u0001\u0001\u0001 2 \u0002  are 2 2 \u0001  2 \u0002\u0002 how 2 2 \u0001  2 \u0002\u0002 hello 1 2 \u0002\u0001 you 2 2 \u0001  2 \u0002\u0002 hi 1 2 \u0001  banana 3 2  \u0002 2 \u0001\u0004 2 \u0002\u0004 apple 2 2  \u0001 2 \u0002\u0003 morning 2 2    2 \u0002\u0005 good 2 2    2 \u0002\u0005 ");
    Searcher search = Searcher("output", 5, 0.75);
    std::stringstream ask("bebe AND banana");
    std::stringstream answer;
    search.TestOneQuery(index, ask, answer);
    ASSERT_EQ(answer.str(), "No such word found\n");
}

TEST(SearcherTestSuite, ManyOrSearchTest) {
    std::stringstream index("3 10 aboba.txt 4 aurora.txt 8 misha.txt 11 bebe 2 4 \u0001\u0001\u0001\u0001 2 \u0002  are 2 2 \u0001  2 \u0002\u0002 how 2 2 \u0001  2 \u0002\u0002 hello 1 2 \u0002\u0001 you 2 2 \u0001  2 \u0002\u0002 hi 1 2 \u0001  banana 3 2  \u0002 2 \u0001\u0004 2 \u0002\u0004 apple 2 2  \u0001 2 \u0002\u0003 morning 2 2    2 \u0002\u0005 good 2 2    2 \u0002\u0005 ");
    Searcher search = Searcher("output", 5, 0.75);
    std::stringstream ask("bebe OR you OR are");
    std::stringstream answer;
    search.TestOneQuery(index, ask, answer);
    ASSERT_EQ(answer.str(), "Found your search in files:\n1. misha.txt on lines: 3 33 \n2. aurora.txt on lines: 2 3 4 33 \n");
}

TEST(SearcherTestSuite, BktPrioritySearchTest) {
    std::stringstream index("3 10 aboba.txt 4 aurora.txt 8 misha.txt 11 bebe 2 4 \u0001\u0001\u0001\u0001 2 \u0002  are 2 2 \u0001  2 \u0002\u0002 how 2 2 \u0001  2 \u0002\u0002 hello 1 2 \u0002\u0001 you 2 2 \u0001  2 \u0002\u0002 hi 1 2 \u0001  banana 3 2  \u0002 2 \u0001\u0004 2 \u0002\u0004 apple 2 2  \u0001 2 \u0002\u0003 morning 2 2    2 \u0002\u0005 good 2 2    2 \u0002\u0005 ");
    Searcher search = Searcher("output", 5, 0.75);
    std::stringstream ask("(bebe OR you) AND are");
    std::stringstream answer;
    search.TestOneQuery(index, ask, answer);
    ASSERT_EQ(answer.str(), "Found your search in files:\n1. misha.txt on lines: 3 \n2. aurora.txt on lines: 33 \n");
}



