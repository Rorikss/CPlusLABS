add_executable(${PROJECT_NAME} main.cpp
        ../lib/Adapters.h
        ../lib/Adapters.cpp
        ../lib/Iterators.h
        ../lib/Ranges.h)

target_link_libraries(${PROJECT_NAME} adapters)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})