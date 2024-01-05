cmake_minimum_required(VERSION 3.12)

project(
        labwork5
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 labwork5"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)

#add_executable(new_target lib/Matrix3D.h
#        lib/Matrix3D.h
#        lib/Matrix3D.cpp
#        bin/main.cpp
#        bin/main.cpp)
add_executable(new_target tests/matrix_tests.cpp
        tests/matrix_tests.cpp)
