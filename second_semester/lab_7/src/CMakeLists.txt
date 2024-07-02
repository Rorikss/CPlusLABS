add_executable(labwork7_Rorikss main.cpp ApplicationController.cpp ApplicationController.h)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
        PUBLIC api
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC cpr::cpr
        PUBLIC ftxui::component
)