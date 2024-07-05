add_executable(${PROJECT_NAME} main.cpp
        ../lib/TTaskScheduler.h
        ../lib/Any.h
        )

target_link_libraries(${PROJECT_NAME} scheduler)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})