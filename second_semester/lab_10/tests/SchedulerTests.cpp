//
// Created by Roriks on 4/11/2024.
//
#include "gtest/gtest.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include "../lib/TTaskScheduler.h"

using namespace Scheduler;

TEST(SchedulerTestSuite, DefaultTaskTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](float a, float c) { return (a * c / 5); }, 3, 5);
    ASSERT_EQ(scheduler.getResult<float>(id), 3);
}

TEST(SchedulerTestSuite, DefaultExecuteAllTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](float a, float c) { return (25 * a * c); }, 1, 2);
    scheduler.executeAll();
    ASSERT_EQ(scheduler.getResult<float>(id), 50);
}

TEST(SchedulerTestSuite, DefaultFutureResultTest) {
    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](float a, float c) { return (a * c / 5); }, 3, 5);
    auto id2 = scheduler.add([](float a, float b)
                                    { return b * b + a; }, 2, scheduler.getFutureResult<float>(id1));
    ASSERT_EQ(scheduler.getResult<float>(id2), 11);
}

TEST(SchedulerTestSuite, DefaultTaskOneArgumentTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](std::string a) { return a.size(); }, "hello!");
    ASSERT_EQ(scheduler.getResult<size_t>(id), 6);
}

TEST(SchedulerTestSuite, VectorTaskOneArgumentTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](int a) { return std::vector<int>(5, a); }, 10);
    ASSERT_EQ(scheduler.getResult<std::vector<int>>(id), std::vector<int>({10, 10, 10, 10, 10}));
}

TEST(SchedulerTestSuite, DefaultTaskNoArgumentTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([]() { return 45 + 50 - 100; });
    ASSERT_EQ(scheduler.getResult<int>(id), -5);
}

TEST(SchedulerTestSuite, StringTaskNoArgumentTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([]() {
        std::string a = "ab";
        std::string b = "oba";
        return a + b;
    });
    ASSERT_EQ(scheduler.getResult<std::string>(id), "aboba");
}

TEST(SchedulerTestSuite, LongChainTest) {
    TTaskScheduler scheduler;
    float a = 1;
    float b = -2;
    float c = 0;
    auto id1 = scheduler.add([](float a, float c) { return (-4 * a * c); }, a, c);
    auto id2 = scheduler.add([](float b, float v)
                                        { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b,
                             scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b,
                             scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v)
                                        { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v)
                                        { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));
    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id6), 0);
}

TEST(SchedulerTestSuite, MultiChainTest) {
    TTaskScheduler scheduler;
    auto id1 = scheduler.add([](int a, int c) { return a + c; }, 3, 5);
    auto id2 = scheduler.add([](int a)
                              { return a * 10; }, scheduler.getFutureResult<int>(id1));
    auto id3 = scheduler.add([](){ return 4 + 6; });
    auto id4 = scheduler.add([](int a, int b)
            { return a + b; }, scheduler.getFutureResult<int>(id2), scheduler.getFutureResult<int>(id3));
    scheduler.executeAll();
    ASSERT_EQ(scheduler.getResult<int>(id4), 90);
}

TEST(SchedulerTestSuite, CastTypeFailureTest) {
    TTaskScheduler scheduler;
    auto id = scheduler.add([](float a, float c) { return (a * c / 5); }, 3, 5);
    scheduler.getResult<float>(id);
    ASSERT_ANY_THROW(scheduler.getResult<int>(id););
}

TEST(MyAnyTestSuite, DefaultIntTest) {
    Any any;
    any = 5;
    ASSERT_EQ(any.Cast<int>(), 5);
}

TEST(MyAnyTestSuite, DefaultStringTest) {
    Any any;
    std::string s = "aboba";
    any = s;
    ASSERT_EQ(any.Cast<std::string>(), "aboba");
}

TEST(MyAnyTestSuite, CopyTest) {
    Any any_1;
    any_1 = 5;
    Any any_2 = any_1;
    ASSERT_EQ(any_2.Cast<int>(), 5);
}

TEST(MyAnyTestSuite, TypeChangeTest) {
    std::stringstream output;
    Any any;
    int a = 5;
    any = a;
    output << any.Cast<int>();
    float b = 7.0;
    any = b;
    output << any.Cast<float>();
    ASSERT_EQ(output.str(), "57");
}

TEST(MyAnyTestSuite, TypeChange2Test) {
    std::stringstream output;
    Any any;
    int a = 28;
    any = a;
    output << any.Cast<int>();
    std::string b = "hello!";
    any = b;
    output << any.Cast<std::string>();
    ASSERT_EQ(output.str(), "28hello!");
}
