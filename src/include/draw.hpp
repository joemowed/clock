#include <array>
#include <rtc.hpp>
class Draw {
  public:
    constexpr static int8_t LOUIE_X_OFFSET = 33;
    constexpr static int8_t LOUIE_Y_OFFSET = -2;
    constexpr static uint8_t BRIGHTNESS_HIGH = 200;
    constexpr static uint8_t BRIGHTNESS_LOW = 5;
    Draw() = delete;
    Draw(Draw &) = delete;
    Draw(Draw &&) = delete;
    ~Draw() = delete;
    static void update();
    static bool updateFlag;

  private:
    static bool initialized;
    static void init();
    static void drawTime();
    static uint8_t reset_hour;
    static void drawLouie();
    static void drawDate();
    static void hourlyReset();
    static void drawDay();
    static void updateBrightness();
    static RTC::RTCData time;
    constexpr static std::array<char[4], 7> WK_DAYS = {"MON", "TUE", "WED", "THR", "FRI", "SAT", "SUN"};
    constexpr static std::array<char[4], 12> MONTHS = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
    static uint8_t digitToASCII(const uint8_t value);
};
