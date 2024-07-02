#pragma once
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <thread>
#include <chrono>
#include "../api/WeatherApiAdapter.h"

class ApplicationController {
public:
    ApplicationController(const WeatherApiAdapter& weather_forecast, size_t freq, size_t days);
    ftxui::Component GetEventComponent(ftxui::Component& renderer);
    void Run();

private:
    void UpdateWeatherInfo();
    void UpdateIfSuccess(const std::vector<std::string>& updated_data);
    void UpdateIfFailure();
    bool OnUpdate();

    WeatherApiAdapter weather_api_;
    std::vector<ftxui::Color> colors_ = {ftxui::Color::Red, ftxui::Color::Orange3,
                                         ftxui::Color::Yellow, ftxui::Color::Green,
                                         ftxui::Color::BlueLight, ftxui::Color::Blue,
                                         ftxui::Color::Purple};
    size_t current_color_index_ = -1;
    ftxui::Element weather_data_ = ftxui::vbox(ftxui::text("weather data can be reached right now"));
    int frequency_;
    bool disconnected_ = false;
    size_t current_city_index_ = 0;
    size_t days_to_forecast_ = 1;
    size_t kEven = 3;
    size_t kColorNumber = 7;
    ftxui::ScreenInteractive screen_ = ftxui::ScreenInteractive::TerminalOutput();
};

