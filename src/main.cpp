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

    Display::updatePixel(0x1, 0, 0);
    Display::updatePixel(0x1, 1, 1);
    Display::updatePixel(0x1, 2, 2);
    Display::updatePixel(0x1, 3, 3);
    Display::updatePixel(0x1, 4, 4);
    Display::updatePixel(0x1, 5, 5);
    Display::updatePixel(0x1, 6, 6);
    Display::updatePixel(0x1, 7, 7);
    Display::updatePixel(0x1, 8, 8);
    Display::updatePixel(0x1, 9, 9);
    Display::updatePixel(0x1, 10, 10);
    Display::updatePixel(0x1, 11, 11);
    Display::updatePixel(0x1, 12, 12);
    Display::updatePixel(0x1, 13, 13);
    Display::updatePixel(0x1, 14, 14);
    Display::updatePixel(0x1, 15, 15);
    Display::updatePixel(0x1, 16, 16);
    Display::updatePixel(0x1, 17, 17);
    Display::updatePixel(0x1, 18, 18);
    Display::updatePixel(0x1, 19, 19);
    Display::updatePixel(0x1, 20, 20);
    Display::updatePixel(0x1, 21, 21);
    Display::updatePixel(0x1, 22, 22);
    Display::updatePixel(0x1, 23, 23);
    Display::updatePixel(0x1, 24, 24);
    Display::updatePixel(0x1, 25, 25);
    Display::updatePixel(0x1, 26, 26);
    Display::updatePixel(0x1, 27, 27);
    Display::updatePixel(0x1, 28, 28);
    Display::updatePixel(0x1, 29, 29);
    Display::updatePixel(0x1, 30, 30);
    Display::updatePixel(0x1, 31, 31);
    while (true) {
    }
    return 0;
}
