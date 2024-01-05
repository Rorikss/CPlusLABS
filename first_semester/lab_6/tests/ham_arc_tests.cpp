#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "..\lib\HamArc.h"

TEST(HamArcTestSuite, MakeBytesTest) {
    ByteAndBitOperations coder;
    char a[3] = {'a', 'b', 'c'};
    std::vector<bool> data = coder.EncodeBytesToBoolVector(a);
    std::vector<char> ans = coder.MakeChunkOfBytes(data);
    for (size_t i = 0; i < ans.size(); ++i) {
        ASSERT_EQ(ans[i], a[i]);
    }
}

TEST(HamArcTestSuite, MakeCharsOfNameTest) {
    ByteAndBitOperations coder;
    char a[3] = {'a', 'b', 'c'};
    std::vector<bool> data = coder.EncodeBytesToBoolVector(a);
    std::string ans = coder.MakeCharsOfName(data);
    for (size_t i = 0; i < ans.size(); ++i) {
        ASSERT_EQ(ans[i], a[i]);
    }
}

TEST(HamArcTestSuite, HammingEncodeTest) {
    Hamming hamming_coder;
    ByteAndBitOperations coder;
    char a[3] = {'a', 'b', 'c'};
    std::vector<bool> data = coder.EncodeBytesToBoolVector(a);
    std::vector<bool> ham = hamming_coder.HammingEncode(data);
    std::vector<bool> ans = {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1,
                             1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1};
    ASSERT_EQ(ham, ans);
}

TEST(HamArcTestSuite, HammingDecodeTest) {
    Hamming hamming_coder;
    ByteAndBitOperations coder;
    char a[4] = {'b', 'o', 'b', 'a'};
    std::vector<bool> data = coder.EncodeBytesToBoolVector(a);
    for (int i = 1; i < data.size(); i++) {
        std::vector<bool> ham = hamming_coder.HammingEncode(data);
        std::vector<bool> back = hamming_coder.HammingDecode(ham);
        ASSERT_EQ(back, data);
    }
}

TEST(HamArcTestSuite, HammingFixTest) {
    Hamming hamming_coder;
    ByteAndBitOperations coder;
    char a[3] = {'k', 'l', 'm'};
    std::vector<bool> data = coder.EncodeBytesToBoolVector(a);
    for (int i = 1; i < data.size(); i++) {
        std::vector<bool> ham = hamming_coder.HammingEncode(data);
        ham[i] = !ham[i]; // breaking some bit
        std::vector<bool> back = hamming_coder.HammingDecode(ham);
        ASSERT_EQ(back, data);
    }
}

TEST(HamArcTestSuite, FileSizeTest) {
    FileOperation file_operator = FileOperation();
    std::string str = "Hello World!";
    std::istringstream in(str);
    uint64_t size = file_operator.GetFileSize(in);
    ASSERT_EQ(size, str.size());
}

TEST(HamArcTestSuite, WriteTillEndTest) {
    FileOperation file_operator = FileOperation();
    std::string str = "Hello World!";
    std::istringstream in(str);
    std::ostringstream out;
    file_operator.WriteTillEndOfFile(in, out);
    ASSERT_EQ(out.str(), str);
}

TEST(HamArcTestSuite, HamArcCreateArchiveChunk3Test) {
    std::ostringstream out;
    std::ofstream a("one.txt");
    for (auto character : "Hello? hello!") a << character;
    std::ofstream b("two.txt");
    for (auto character : "Bye-bye!!!") b << character;
    HamArc archiver("archive_test", 3);
    std::vector<std::string> files = {"one.txt", "two.txt"};
    archiver.CreateArchive(files);
    archiver.ListNames(out);
    std::string ans = "Opening archive: \"archive_test.haf\" of size 72\none.txt\ntwo.txt\nThese are all files in current archive\n";
    ASSERT_EQ(out.str(), ans);
}

TEST(HamArcTestSuite, HamArcCreateArchiveChunk4Test) {
    std::ostringstream out;
    std::ofstream a("one.txt");
    for (auto character : "Hello? hello!") a << character;
    std::ofstream b("two.txt");
    for (auto character : "Bye-bye!!!") b << character;
    HamArc archiver("archive_test", 4);
    std::vector<std::string> files = {"one.txt", "two.txt"};
    archiver.CreateArchive(files);
    archiver.ListNames(out);
    std::string ans = "Opening archive: \"archive_test.haf\" of size 80\none.txt\ntwo.txt\nThese are all files in current archive\n";
    ASSERT_EQ(out.str(), ans);
}

TEST(HamArcTestSuite, DeleteChunk4Test) {
    std::ostringstream out;
    HamArc archiver("archive_test", 4);
    std::string file_to_delete = "two.txt";
    archiver.DeleteFile(file_to_delete);
    archiver.ListNames(out);
    std::string ans = "Opening archive: \"archive_test.haf\" of size 40\none.txt\nThese are all files in current archive\n";
    ASSERT_EQ(out.str(), ans);
}

TEST(HamArcTestSuite, AppendFileChunk4Test) {
    std::ostringstream out;
    std::ofstream input("three.txt");
    for (auto character : "ITS ME.") input << character;
    HamArc archiver("archive_test", 4);
    archiver.AppendOneFile("three.txt");
    archiver.ListNames(out);
    std::string ans = "Opening archive: \"archive_test.haf\" of size 80\none.txt\nthree.txt\nThese are all files in current archive\n";
    ASSERT_EQ(out.str(), ans);
}


TEST(HamArcTestSuite, MergeTest) {
    std::ostringstream out;
    HamArc archiver_1("archive_test_1", 2);
    HamArc archiver_2("archive_test_2", 2);
    archiver_1.CreateArchive({"one.txt", "two.txt"});
    archiver_2.CreateArchive({"three.txt"});
    HamArc archiver_merged("archive_test_merged", 2);
    archiver_merged.Merge("archive_test_1.haf", "archive_test_2.haf");
    archiver_merged.ListNames(out);
    std::string ans = "Opening archive: \"archive_test_merged.haf\" of size 168\none.txt\ntwo.txt\nthree.txt\nThese are all files in current archive\n";
    ASSERT_EQ(out.str(), ans);
}
