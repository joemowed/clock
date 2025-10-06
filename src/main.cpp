#include <array>
#include <cstdint>
#include <samd51j20a.h>

#include <display.hpp>
#include <transcode.hpp>
#include <utility.hpp>
void loadLouie() {
    auto louie = Transcode::transcodeLouie();
    for (int y = 0; y < LOUIE_HEIGHT; y++) {
        for (int x = 0; x < LOUIE_WIDTH; x++) {
            const uint8_t &curr_pixel = louie.at(x + (y * LOUIE_WIDTH));
            Display::updatePixel(curr_pixel, x, y);
        }
    }
    for (int x = 0; x < LOUIE_WIDTH; x++) {
        Display::updatePixel(0x0, x, 0);
    }
}
int main(void) {
    Display::init();
    Display::fill(0x0);
    // loadLouie();
    Display::updatePixel(0x4, 0, 1);
    Display::updatePixel(0x1, 1, 0);
    Display::updatePixel(0x4, 1, 5);
    Display::updatePixel(0x1, 5, 1);
    while (true) {
    }
    return 0;
}
