#include <cstdint>
#include <ctime>
class RTC {
  public:
    typedef struct RTCData {
        uint8_t sec_ones;
        uint8_t sec_tens;
        uint8_t min_ones;
        uint8_t min_tens;
        uint8_t hour_ones;
        uint8_t hour_tens;
        uint8_t wkday;
        uint8_t date_ones;
        uint8_t date_tens;
        uint8_t month_ones;
        uint8_t month_tens;
        uint8_t year_ones;
        uint8_t year_tens;
        bool PM;
    } RTCData;
    static void init();
    static void init(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wkday, uint8_t date, uint8_t month, uint8_t year, bool PM);
    static constexpr const bool OSC_ENABLE = 1;
    static constexpr const bool BATT_BKP_ENABLE = 1;
    static constexpr const bool TWELVE_HR_MODE = 1;
    RTC() = delete;
    RTC(RTC &) = delete;
    RTC(RTC &&) = delete;
    ~RTC() = delete;
    static RTCData now();

  private:
    static uint8_t getTens(const uint8_t value);
    static uint8_t getOnes(const uint8_t value);
    static uint8_t getTotal(const uint8_t tens, const uint8_t ones);
    static uint8_t update_divisor;
    static bool checkDayLightSavings(const RTCData &time);
    static bool isTimeWithinRange(std::time_t time, std::time_t start, std::time_t end);
    static std::time_t makeTime(int year, int month, int day, int hour, int min, int sec);
    static std::time_t makeTime(const RTCData &time);
    static RTCData makeTime(std::time_t time);
};
