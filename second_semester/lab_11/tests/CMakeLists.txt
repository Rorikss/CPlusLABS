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
        search_engine_tests
        SearchEngineTests.cpp
        ../lib/Indexer.h
        ../lib/Indexer.cpp
        ../lib/Searcher.cpp
        ../lib/Searcher.h
        ../lib/VarInt.h
        ../lib/InfoStructures.h
        ../lib/SyntaxParser.h
        ../lib/SyntaxParser.cpp
        ../lib/TermSearch.cpp
        ../lib/TermSearch.h
)

target_link_libraries(
        search_engine_tests
        SearchEngine
        GTest::gtest_main
)

target_include_directories(search_engine_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(search_engine_tests)