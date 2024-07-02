#pragma once
#include <ftxui/component/component.hpp>

enum class WeatherCode {
    Clear,
    Cloudy,
    Snow,
    Rain,
    Fog,
    Showers,
    Drizzle,
    Storm,
    Default
};

class ConsolePrint {
public:
    static ftxui::Element GetPicture(WeatherCode code);
    static ftxui::Element GetDateText(const std::string& line);

private:
    static ftxui::Element Cloudy();
    static ftxui::Element Fog();
    static ftxui::Element Rain();
    static ftxui::Element Showers();
    static ftxui::Element Drizzle();
    static ftxui::Element Snow();
    static ftxui::Element Clear();
    static ftxui::Element Storm();
};