cmake_minimum_required(VERSION 3.12)

project(
        labwork-9
        VERSION 1.0
        DESCRIPTION "C++ Year 2024 labwork9"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(tests)