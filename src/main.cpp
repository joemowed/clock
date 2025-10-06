#include <array>
#include <cstdint>
#include <samd51j20a.h>

#include <display.hpp>
#include <utility.hpp>
int main(void) {
    Display::init();

    uint8_t x = 0;
    uint8_t y = 0;
    while (true) {
        x = (x >= Display::line_size) ? x : 0;
        y = (y >= Display::line_count) ? y : 0;

        Display::fill(0x2);
        // Display::updatePixel(0x6, x, y);
    }
    return 0;
}
