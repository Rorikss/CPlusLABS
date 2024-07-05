//
// Created by Roriks on 4/10/2024.
//
#include "./lib/TTaskScheduler.h"
#include <cmath>

using namespace Scheduler;


int main() {
    TTaskScheduler scheduler;

    float a = 1;
    float b = -2;
    float c = 0;
    auto id1 = scheduler.add([](float a, float c) { return (-4 * a * c); }, a, c);
    scheduler.executeAll();
    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));
    scheduler.executeAll();
    auto id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b,
                             scheduler.getFutureResult<float>(id2));
    scheduler.executeAll();
    auto id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b,
                             scheduler.getFutureResult<float>(id2));
    scheduler.executeAll();
    auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));
    scheduler.executeAll();
    auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));
    scheduler.executeAll();
    auto id7 = scheduler.add([](int a) { return a + 5; }, 5);

    scheduler.executeAll();
    std::cout << scheduler.getResult<float>(id1) << std::endl;
    std::cout << scheduler.getResult<float>(id2) << std::endl;
    std::cout << scheduler.getResult<float>(id3) << std::endl;
    std::cout << scheduler.getResult<float>(id4) << std::endl;
    std::cout << scheduler.getResult<float>(id5) << std::endl;
    std::cout << scheduler.getResult<float>(id1) << std::endl;
    std::cout << scheduler.getResult<int>(id7) << std::endl;

    return 0;
}