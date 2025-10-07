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
    Display::init();
    loadLouie(32, -2);
    Font::drawStr("JOHNNY", 6, 0x5, 0, 0);
    Font::drawStr("HAS NO", 6, 0x6, 0, 1);
    Font::drawStr(" DUPR ", 6, 0x7, 0, 2);

    while (true) {
    }
    return 0;
}
