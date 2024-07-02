add_library(api WeatherApiAdapter.cpp WeatherApiAdapter.h CityApiAdapter.cpp CityApiAdapter.h
        ConsolePrint.cpp
        ConsolePrint.h)

target_link_libraries(api
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC cpr::cpr
        PUBLIC ftxui::component
)
