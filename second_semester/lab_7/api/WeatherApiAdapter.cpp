#include "WeatherApiAdapter.h"

WeatherApiAdapter::WeatherApiAdapter(const std::vector<std::string>& city_names, const std::string& api_key, int days)
        : CityApiAdapter(city_names, api_key), days_(days)
        {}

std::vector<std::string> WeatherApiAdapter::FetchForecast(size_t index) {
    cpr::Response response = Request(index);
    if (response.status_code != kSuccessCode) {
        return std::vector<std::string>{};
    }
    json response_data = json::parse(response.text);
    if (response_data.empty()) {
        return std::vector<std::string>{};
    }
    return ParseToForecast(response_data);
}

void WeatherApiAdapter::IncreaseDays() {
    days_++;
}

void WeatherApiAdapter::DecreaseDays() {
    days_ = std::max(1, days_ - 1);
}

ftxui::Element WeatherApiAdapter::GetPictureOfWeather(size_t index) {
    return PictureByCode(weather_codes_[index]);
}

ftxui::Element WeatherApiAdapter::PictureByCode(WeatherCode code) {
    return ConsolePrint::GetPicture(code);
}

std::string WeatherApiAdapter::ParseWeatherType(size_t code) {
    switch (code) {
        case 0:
            weather_codes_.push_back(WeatherCode::Clear);
            return "Clear Sky ☀\uFE0F";
        case 1:
            weather_codes_.push_back(WeatherCode::Cloudy);
            return "Mainly clear 🌤️";
        case 3:
        case 2:
            weather_codes_.push_back(WeatherCode::Cloudy);
            return "Mainly clear 🌤️";
        case 45:
        case 48:
            weather_codes_.push_back(WeatherCode::Fog);
            return "Fog and depositing rime fog 🌫️";
        case 51:
            weather_codes_.push_back(WeatherCode::Drizzle);
            return "Light drizzle 🌧️";
        case 53:
            weather_codes_.push_back(WeatherCode::Drizzle);
            return "Moderate drizzle 🌧️🌧️";
        case 55:
            weather_codes_.push_back(WeatherCode::Drizzle);
            return "Dense drizzle 🌧️🌧️🌧️";
        case 56:
        case 57:
            weather_codes_.push_back(WeatherCode::Drizzle);
            return "Freezing drizzle ❄️🌧️";
        case 61:
            weather_codes_.push_back(WeatherCode::Rain);
            return "Light rain 🌧️";
        case 63:
            weather_codes_.push_back(WeatherCode::Rain);
            return "Moderate rain 🌧️🌧️";
        case 65:
            weather_codes_.push_back(WeatherCode::Rain);
            return "Heavy rain 🌧️🌧️🌧️";
        case 66:
        case 67:
            weather_codes_.push_back(WeatherCode::Rain);
            return "Freezing rain ❄️🌧️";
        case 71:
            weather_codes_.push_back(WeatherCode::Snow);
            return "Light snow ❄️";
        case 73:
            weather_codes_.push_back(WeatherCode::Snow);
            return "Moderate snow ❄️❄️️";
        case 75:
            weather_codes_.push_back(WeatherCode::Snow);
            return "Heavy snow ❄️❄️❄️";
        case 77:
            weather_codes_.push_back(WeatherCode::Snow);
            return "Snow grains ❄️❄️❄️";
        case 80:
        case 81:
        case 82:
            weather_codes_.push_back(WeatherCode::Showers);
            return "Rain showers 🌧️🌧️🌧️🌧️";
        case 85:
        case 86:
            weather_codes_.push_back(WeatherCode::Showers);
            return "Snow showers ❄️❄️❄️❄️";
        case 95:
        case 96:
        case 99:
            weather_codes_.push_back(WeatherCode::Storm);
            return "Thunder storm ⛈️";
        default:
            weather_codes_.push_back(WeatherCode::Default);
            return "Unknown weather 👽";
    }
}

std::vector<std::string> WeatherApiAdapter::ParseToForecast(const json& parsed_data) {
    std::vector<std::string> dates = parsed_data["daily"]["time"];
    std::vector<float> temperatures = parsed_data["daily"]["temperature_2m_max"];
    std::vector<int> weather = parsed_data["daily"]["weather_code"];
    std::vector<float> wind = parsed_data["daily"]["wind_speed_10m_max"];

    std::vector<std::string> forecasts;
    weather_codes_.clear();
    for (size_t i = 0; i < days_; ++i) {
        forecasts.push_back("DATE " + dates[i]);
        forecasts.push_back("TEMPERATURE: " + MakeBeautifulFloat(temperatures[i]) + "°C");
        forecasts.push_back("WIND SPEED: " + MakeBeautifulFloat(wind[i]));
        forecasts.push_back(ParseWeatherType(weather[i]));
    }
    return forecasts;
}

cpr::Response WeatherApiAdapter::Request(size_t index) {
    cpr::Response response = cpr::Get(
            cpr::Url{weather_api_},
            cpr::Parameters{{"latitude", std::to_string(GetCityLat(index))},
                            {"longitude", std::to_string(GetCityLon(index))},
                            {"daily", "temperature_2m_max"},
                            {"daily", "weather_code"},
                            {"daily", "wind_speed_10m_max"},
                            {"forecast_days", std::to_string(days_)}}
    );
    return response;
}

std::string WeatherApiAdapter::MakeBeautifulFloat(float num) {
    std::stringstream ss;
    ss << num;
    std::string str = ss.str();
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    return str;
}
