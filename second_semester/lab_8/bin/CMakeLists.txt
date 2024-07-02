add_executable(${PROJECT_NAME} main.cpp
        ../lib/BST.h
        ../lib/BST.cpp)

target_link_libraries(${PROJECT_NAME} container)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})