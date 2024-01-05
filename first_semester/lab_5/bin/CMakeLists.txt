add_executable(${PROJECT_NAME} main.cpp
        ../lib/Matrix3D.h)

target_link_libraries(${PROJECT_NAME} PRIVATE MatrixArray)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})