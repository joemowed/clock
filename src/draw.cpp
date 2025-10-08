#include <display.hpp>
#include <draw.hpp>
#include <font.hpp>
#include <setup_tc0.hpp>
#include <transcode.hpp>
RTC::RTCData Draw::time = {};
bool Draw::updateFlag = true;
bool Draw::initialized = false;
uint8_t Draw::reset_hour = 0;
void Draw::drawLouie() {
    auto louie = Transcode::transcodeLouie();
    for (int y = 0; y < LOUIE_HEIGHT; y++) {
        for (int x = 0; x < LOUIE_WIDTH; x++) {
            const uint8_t &curr_pixel = louie.at(x + (y * LOUIE_WIDTH));
            uint8_t x_final = x + LOUIE_X_OFFSET;
            uint8_t y_final = y + LOUIE_Y_OFFSET;
            if ((x_final < LOUIE_X_OFFSET) | (x_final > 63)) {
                x_final = 63;
            } else if ((y_final > 18) && (x_final < 36)) {
                x_final = 63;
            }
            Display::updatePixel(curr_pixel, x_final, y_final);
        }
    }
}
void Draw::init() {
    configureGCLKForTC0();
    initTC0();
    initialized = true;
}
void Draw::update() {
    if (!updateFlag) {
        return;
    }
    updateFlag = false;
    if (!initialized) {
        init();
    }
    time = RTC::now();
    drawLouie();
    drawDay();
    drawTime();
    drawDate();
    updateBrightness();
    hourlyReset();
}
void Draw::hourlyReset() {

    const uint8_t hour = (time.hour_tens * 10) + time.hour_ones;
    if (time.reset_hour != hour) {
        RTC::writeResetHour(hour);
        NVIC_SystemReset();
    }
}
void Draw::updateBrightness() {
    const uint8_t hour = (time.hour_tens * 10) + time.hour_ones;
    const uint8_t min = (time.min_tens * 10) + time.min_ones;
    if ((hour >= 10) && (time.PM == true)) {
        Display::setBrightness(BRIGHTNESS_LOW); // low brightness at 10PM
        return;
    }
    if ((hour <= 6) && (time.PM == false)) {
        if ((min <= 45)) {
            Display::setBrightness(BRIGHTNESS_LOW); // low brightness stops at 6:45AM
            return;
        }
    }
    Display::setBrightness(BRIGHTNESS_HIGH); // otherwise high brightness
}

void Draw::drawDay() {
    uint8_t wk_days_index = time.wkday - 1;
    wk_days_index = (wk_days_index > 6) ? 6 : wk_days_index;
    const char *day = WK_DAYS[wk_days_index];
    static char str[5] = {};
    str[0] = ' ';
    for (int i = 0; i < 3; i++) {
        str[i + 1] = day[i];
    }
    Font::drawStr(str, 4, 0x5, 3, 0);
}
void Draw::drawTime() {
    static char str[6] = {};
    const uint8_t hours_tens_char = (time.hour_tens == 0) ? ' ' : digitToASCII(time.hour_tens);
    str[0] = hours_tens_char;
    str[1] = digitToASCII(time.hour_ones);
    str[2] = ':';
    str[3] = digitToASCII(time.min_tens);
    str[4] = digitToASCII(time.min_ones);
    if (hours_tens_char == ' ') {
        Font::drawStr(str, 5, 0x7, 0, 1);
    } else {
        Font::drawStr(str, 5, 0x7, 3, 1);
    }
}
void Draw::drawDate() {

    static char str[7] = {};
    uint8_t month_index = (time.month_tens * 10) + (time.month_ones) - 1;
    month_index = (month_index > 11) ? 11 : month_index;
    const char *month = MONTHS[month_index];

    for (int i = 0; i < 3; i++) {
        str[i] = month[i];
    }
    str[3] = ' ';
    if (time.date_tens == 0) {
        str[4] = digitToASCII(time.date_ones);
        str[5] = ' ';
        Font::drawStr(str, 6, 0x6, 3, 2);
    } else {
        str[4] = digitToASCII(time.date_tens);
        str[5] = digitToASCII(time.date_ones);
        Font::drawStr(str, 6, 0x6, 0, 2);
    }
}
uint8_t Draw::digitToASCII(const uint8_t value) { return value + 48; }
