#pragma once
#include "CityApiAdapter.h"
#include "ConsolePrint.h"
#include <ftxui/component/component.hpp>
#include <cmath>

class WeatherApiAdapter : public CityApiAdapter {
public:
    WeatherApiAdapter(const std::vector<std::string>& city_names, const std::string& api_key, int days);
    std::vector<std::string> FetchForecast(size_t index);
    void IncreaseDays();
    void DecreaseDays();
    ftxui::Element GetPictureOfWeather(size_t index);

private:
    ftxui::Element PictureByCode(WeatherCode code);
    std::string ParseWeatherType(size_t code);
    std::vector<std::string> ParseToForecast(const json& parsed_data);
    cpr::Response Request(size_t index);
    std::string MakeBeautifulFloat(float num);

    int days_ = 1;
    std::vector<WeatherCode> weather_codes_;
    std::string weather_api_ = "https://api.open-meteo.com/v1/forecast";
};
