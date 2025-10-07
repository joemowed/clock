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
void drawChar(const uint8_t character, const uint8_t color) {
    Font::DecompChar curr_char = Font::getChar(character);
    for (int y = 0; y < 8; y++) {
        const uint8_t curr_row = curr_char.at(y);
        for (int x = 0; x < 5; x++) {
            if (curr_row & (1 << x)) {
                Display::updatePixel(color, x, y);

            } else {
                Display::updatePixel(0x0, x, y);
            }
        }
    }
}
int main(void) {
    Display::init();
    Display::fill(0x7);
    drawChar('A', 0x2);
    loadLouie(32, -2);

    while (true) {
    }
    return 0;
}
