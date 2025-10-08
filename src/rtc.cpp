#include <ctime>
#include <i2c.hpp>
#include <rtc.hpp>
void RTC::init() { I2C::init(); }
void RTC::init(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wkday, uint8_t date, uint8_t month, uint8_t year, bool PM) {
    init();

    I2C::write(0x0, (OSC_ENABLE << 7) | (getTens(sec) << 4) | (getOnes(sec) << 0));                   // RTCSEC
    I2C::write(0x1, (getTens(min) << 4) | (getOnes(min) << 0));                                       // RTCMIN
    I2C::write(0x2, (TWELVE_HR_MODE << 6) | (PM << 5) | (getTens(hour) << 4) | (getOnes(hour) << 0)); // RTCHOUR
    I2C::write(0x3, (BATT_BKP_ENABLE << 3) | (getOnes(wkday) << 0));                                  // RTCWKDAY
    I2C::write(0x4, (getTens(date) << 4) | (getOnes(date) << 0));                                     // RTCDATE
    I2C::write(0x5, (getTens(month) << 4) | (getOnes(month) << 0));                                   // RTCMONTH
    I2C::write(0x6, (getTens(year) << 4) | (getOnes(year) << 0));                                     // RTCYEAR
    I2C::write(0x20, 0);                                                                              // reset_hour
}
RTC::RTCData RTC::now() {
    RTCData ret = {};
    const uint8_t sec_reg = I2C::read(0x0);
    const uint8_t min_reg = I2C::read(0x1);
    const uint8_t hour_reg = I2C::read(0x2);
    const uint8_t wkday_reg = I2C::read(0x3);
    const uint8_t date_reg = I2C::read(0x4);
    const uint8_t month_reg = I2C::read(0x5);
    const uint8_t year_reg = I2C::read(0x6);
    const uint8_t reset_hour_reg = I2C::read(0x20);

    ret.sec_ones = (sec_reg >> 0) & 0b1111;
    ret.sec_tens = (sec_reg >> 4) & 0b111;
    ret.min_ones = (min_reg >> 0) & 0b1111;
    ret.min_tens = (min_reg >> 4) & 0b111;
    ret.hour_ones = (hour_reg >> 0) & 0b1111;
    ret.hour_tens = (hour_reg >> 4) & 0b1;
    ret.wkday = (wkday_reg >> 0) & 0b111;
    ret.date_ones = (date_reg >> 0) & 0b1111;
    ret.date_tens = (date_reg >> 4) & 0b11;
    ret.month_ones = (month_reg >> 0) & 0b1111;
    ret.month_tens = (month_reg >> 4) & 0b1;
    ret.year_ones = (year_reg >> 0) & 0b1111;
    ret.year_tens = (year_reg >> 4) & 0b1111;
    ret.PM = (hour_reg >> 5) & 0b1;
    ret.reset_hour = (reset_hour_reg >> 0) & 0xFF;

    return ret;
}
uint8_t RTC::getOnes(uint8_t value) { return value % 10; }
uint8_t RTC::getTens(uint8_t value) { return (value / 10) % 10; }
uint8_t RTC::getTotal(uint8_t tens, uint8_t ones) { return (tens * 10) + ones; }
void RTC::writeResetHour(const uint8_t reset_hour) { I2C::write(0x20, reset_hour); }
