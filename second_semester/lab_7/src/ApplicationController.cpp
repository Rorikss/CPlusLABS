#include "ApplicationController.h"


ApplicationController::ApplicationController(const WeatherApiAdapter& weather_forecast, size_t freq, size_t days)
        : weather_api_(weather_forecast),
          days_to_forecast_(days),
          frequency_(freq)
        {}

ftxui::Component ApplicationController::GetEventComponent(ftxui::Component& renderer) {
    return CatchEvent(renderer, [&](ftxui::Event event) {
        if (event == ftxui::Event::Character('q') || event == ftxui::Event::Escape) {
            screen_.ExitLoopClosure()();
            return true;
        } else if (event == ftxui::Event::Character('n')) {
            current_city_index_ = (current_city_index_ + 1) % days_to_forecast_;
            return OnUpdate();
        } else if (event == ftxui::Event::Character('p')) {
            if (current_city_index_ <= 0) {
                current_city_index_ += days_to_forecast_ - 1;
            } else {
                current_city_index_--;
            }
            return OnUpdate();
        } else if (event == ftxui::Event::Character('+')) {
            weather_api_.IncreaseDays();
            return OnUpdate();
        } else if (event == ftxui::Event::Character('-')) {
            weather_api_.DecreaseDays();
            return OnUpdate();
        }
        UpdateWeatherInfo();
        return false;
    });
}

void ApplicationController::Run() {
    UpdateWeatherInfo();
    auto renderer = ftxui::Renderer([&] {
        return window(ftxui::text("Current city: " +
                        weather_api_.GetCurrentCityName(current_city_index_)) | ftxui::bold | ftxui::center,
                      weather_data_);});
    auto component = GetEventComponent(renderer);
    auto update_text = [&]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(frequency_));
            //UpdateWeatherInfo();
            screen_.PostEvent(ftxui::Event::Custom);
        }
    };
    std::thread update_thread(update_text);
    update_thread.detach();
    screen_.Loop(component);
}

void ApplicationController::UpdateWeatherInfo() {
    std::vector<std::string> updated_data = weather_api_.FetchForecast(current_city_index_);
    if (!updated_data.empty()) {
        UpdateIfSuccess(updated_data);
        if (disconnected_) {
            disconnected_ = false;
        }
        return;
    }
    if (!disconnected_) {
        UpdateIfFailure();
    }
}

void ApplicationController::UpdateIfSuccess(const std::vector<std::string>& updated_data) {
    ftxui::Elements elements;
    ftxui::Elements day;
    size_t line_number = 0;
    size_t day_num = 0;
    current_color_index_ = -1;
    for (const auto& line : updated_data) {
        if (line_number == 0) {
            elements.push_back(ConsolePrint::GetDateText(line));
            current_color_index_ = (current_color_index_ + 1) % kColorNumber;
        } else {
            day.push_back(ftxui::text(line) | color(colors_[current_color_index_]));
            if (line_number == kEven) {
                line_number = -1;
                elements.push_back(hbox(ftxui::separatorEmpty(), weather_api_.GetPictureOfWeather(day_num) | ftxui::center,
                                        ftxui::separatorCharacter("   "), vbox(day) | ftxui::center) | ftxui::hcenter);
                day.clear();
                day_num++;
            }
        }
        line_number++;
    }
    weather_data_ = vbox(std::move(elements));
}

void ApplicationController::UpdateIfFailure() {
    weather_data_ = vbox(ftxui::text("You are offline or an error occurred, data isn't up-to-date"),
                         ftxui::separatorEmpty(), weather_data_);
    disconnected_ = true;
}

bool ApplicationController::OnUpdate() {
    UpdateWeatherInfo();
    return true;
}

