include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        ham_arc_tests
        ham_arc_tests.cpp
        ../lib/HamArc.h
)

target_link_libraries(
        ham_arc_tests
        HamArc
        GTest::gtest_main
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_include_directories(ham_arc_tests PRIVATE "${PROJECT_SOURCE_DIR}/lib")

include(GoogleTest)

gtest_discover_tests(ham_arc_tests)
