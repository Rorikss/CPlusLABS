cmake_minimum_required(VERSION 3.12)

project(
        labwork-10
        VERSION 1.0
        DESCRIPTION "C++ Year 2024 labwork10"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(lib)
add_subdirectory(bin)
add_subdirectory(tests)