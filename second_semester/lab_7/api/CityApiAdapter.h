#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <cpr/cpr.h> // Для работы с HTTP
#include <nlohmann/json.hpp>


using json = nlohmann::json;
struct Coordinates {
    float lat = 0.0;
    float lon = 0.0;
};
const int kSuccessCode = 200;

class CityApiAdapter {
public:
    CityApiAdapter(const std::vector<std::string>& city_names, const std::string& api_key);
    float GetCityLat(size_t index);
    float GetCityLon(size_t index);
    std::string GetCurrentCityName(size_t index);
    bool Empty();

private:
    std::vector<std::pair<std::string, Coordinates>> city_data_;
    Coordinates GetRequest(const std::string& city);
    void MakeCoordinates(const std::vector<std::string>& city_names);

    std::string api_url_ = "https://api.api-ninjas.com/v1/city?name=";
    std::string api_key_;
};
