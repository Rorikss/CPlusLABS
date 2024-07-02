#include "CityApiAdapter.h"

CityApiAdapter::CityApiAdapter(const std::vector<std::string>& city_names, const std::string& api_key) : api_key_(api_key) {
    MakeCoordinates(city_names);
}

float CityApiAdapter::GetCityLat(size_t index) {
    return city_data_[index].second.lat;
}

float CityApiAdapter::GetCityLon(size_t index) {
    return city_data_[index].second.lon;
}

std::string CityApiAdapter::GetCurrentCityName(size_t index) {
    return city_data_[index].first;
}

bool CityApiAdapter::Empty() {
    return city_data_.empty();
}

Coordinates CityApiAdapter::GetRequest(const std::string& city) {
    cpr::Response response = cpr::Get(
            cpr::Url{api_url_ + city},
            cpr::Header{{"X-API-Key", api_key_}}
    );
    if (response.status_code != kSuccessCode) {
        return Coordinates{};
    }
    json response_json = json::parse(response.text);
    if (!response_json.empty()) {
        Coordinates data;
        data.lat = response_json[0]["latitude"].get<float>();
        data.lon = response_json[0]["longitude"].get<float>();
        return data;
    }
    return Coordinates{};
}

void CityApiAdapter::MakeCoordinates(const std::vector<std::string>& city_names) {
    for (const auto& city : city_names) {
        city_data_.push_back(std::make_pair(city, GetRequest(city)));
    }
}

