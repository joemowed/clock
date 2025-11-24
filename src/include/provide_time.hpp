#include <iostream>
class ProvideTime {
  private:
    // Helper to convert two consecutive characters at index I into an integer
    constexpr int parse_two_digits(const char *str, std::size_t I) { return (str[I] - '0') * 10 + (str[I + 1] - '0'); }

    // Get hours (0-23)
    constexpr int get_hours_24h() { return parse_two_digits(__TIME__, 0); }

  public:
    // Get minutes (0-59)
    constexpr int get_minutes() { return parse_two_digits(__TIME__, 3); }

    // Get seconds (0-59)
    constexpr int get_seconds() { return parse_two_digits(__TIME__, 6); }

    // Determine if it's PM (true) or AM (false) in a 12-hour context
    constexpr bool is_pm() { return get_hours_24h() >= 12; }

    // Get hours in 12-hour format (1-12)
    constexpr int get_hours() {
        int h = get_hours_24h();
        if (h == 0)
            return 12; // Midnight is 12 AM
        if (h > 12)
            return h - 12; // PM hours
        return h;          // AM hours
    }

    // Get day of the month (1-31)
    constexpr int get_day() {
        // Day starts at index 4 (e.g., "Nov 24 2025")
        // Need to handle single-digit days where index 4 might be a space ' '
        if (__DATE__[4] == ' ') {
            return __DATE__[5] - '0';
        }
        return parse_two_digits(__DATE__, 4);
    }

    // Get year (e.g., 25)
    constexpr int get_year() {
        // Year starts at index 7
        return (parse_two_digits(__DATE__, 7) * 100 + parse_two_digits(__DATE__, 9)) - 2000;
    }
};
