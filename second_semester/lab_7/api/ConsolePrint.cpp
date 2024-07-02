#include "ConsolePrint.h"

ftxui::Element ConsolePrint::GetPicture(WeatherCode code) {
    switch (code) {
        case WeatherCode::Clear:
            return Clear();
        case WeatherCode::Cloudy:
            return Cloudy();
        case WeatherCode::Fog:
            return Fog();
        case WeatherCode::Drizzle:
            return Drizzle();
        case WeatherCode::Rain:
            return Rain();
        case WeatherCode::Snow:
            return Snow();
        case WeatherCode::Showers:
            return Showers();
        case WeatherCode::Storm:
            return Storm();
        default:
            return vbox(ftxui::text("no image :("));
    }
}

ftxui::Element ConsolePrint::GetDateText(const std::string& line) {
    return vbox(ftxui::separator(),
                ftxui::text(line) | ftxui::bold | ftxui::center, ftxui::separator());
}

ftxui::Element ConsolePrint::Cloudy() {
    return vbox(
            ftxui::text("     .--.    "),
            ftxui::text("  .-(    ).  "),
            ftxui::text(" (___.__)__) "));
}

ftxui::Element ConsolePrint::Fog() {
    return vbox(
            ftxui::text(" _ - _ - _ - "),
            ftxui::text("  _ - _ - _  "),
            ftxui::text(" _ - _ - _ - "));
}

ftxui::Element ConsolePrint::Rain() {
    return vbox(
            ftxui::text("     .-.     "),
            ftxui::text("    (   ).   "),
            ftxui::text("   (___(__)  "),
            ftxui::text("    ' ' ' '  "),
            ftxui::text("   ' ' ' '   "));
}

ftxui::Element ConsolePrint::Showers() {
    return vbox(
            ftxui::text(" _`/\"\".-.    "),
            ftxui::text("  ,\\_(   ).  "),
            ftxui::text("   /(___(__) "),
            ftxui::text("     ' ' ' ' "),
            ftxui::text("    ' ' ' '  "));
}

ftxui::Element ConsolePrint::Drizzle() {
    return vbox(
            ftxui::text(" _`/\"\".-.    "),
            ftxui::text("  ,\\_(   ).  "),
            ftxui::text("   /(___(__) "),
            ftxui::text("     ' , ' , "),
            ftxui::text("    , ' , '  "));
}

ftxui::Element ConsolePrint::Snow() {
    return vbox(
            ftxui::text("     .-.     "),
            ftxui::text("    (   ).   "),
            ftxui::text("   (___(__)  "),
            ftxui::text("    *  *  *  "),
            ftxui::text("   *  *  *   "));
}

ftxui::Element ConsolePrint::Clear() {
    return vbox(
            ftxui::text("    \\   /    "),
            ftxui::text("     .-.     "),
            ftxui::text("  - (   ) -  "),
            ftxui::text("     `-'     "),
            ftxui::text("    /   \\    "));
}

ftxui::Element ConsolePrint::Storm() {
        return vbox(
                ftxui::text("     .-.     "),
                ftxui::text("    (   ).   "),
                ftxui::text("   (___(__)  "),
                ftxui::text("  ,'7',7,'   "),
                ftxui::text("  ,','7','   "));
}
