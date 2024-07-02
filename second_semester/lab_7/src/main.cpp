#include "ApplicationController.h"
using json = nlohmann::json;

int main() {
    std::ifstream file("config.json");
    if (!file) {
        throw std::runtime_error("config.json file not found");
    }
    json json_data = json::parse(file);
    if (json_data.empty()) {
        throw std::runtime_error("An error occurred while parsing json");
    }
    std::string api_key = json_data["api-key"].get<std::string>();
    int frequency = json_data["freq"].get<int>();
    int days_to_forecast = json_data["days"].get<int>();
    std::vector<std::string> cities = json_data["cities"].get<std::vector<std::string>>();
    WeatherApiAdapter weather_updater = WeatherApiAdapter(cities, api_key, days_to_forecast);
    ApplicationController app = ApplicationController(weather_updater, frequency, cities.size());
    app.Run();
    return 0;
}
