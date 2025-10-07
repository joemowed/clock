#include <array>
#include <cstdint>
#include <display.hpp>
#include <font.hpp>
#include <samd51j20a.h>
#include <transcode.hpp>
#include <utility.hpp>
void loadLouie(const int8_t x_offset, const int8_t y_offset) {
    auto louie = Transcode::transcodeLouie();
    for (int y = 0; y < LOUIE_HEIGHT; y++) {
        for (int x = 0; x < LOUIE_WIDTH; x++) {
            const uint8_t &curr_pixel = louie.at(x + (y * LOUIE_WIDTH));
            Display::updatePixel(curr_pixel, x + x_offset, y + y_offset);
        }
    }
}
int main(void) {
    Font::init();
    Display::init();
    Display::fill(0x7);
    for (uint16_t i = ' '; i < 256; i++) {
        auto curr_char = Font::getChar(i);
        volatile auto i2 = curr_char;
    }
    loadLouie(32, -2);

    while (true) {
    }
    return 0;
}
