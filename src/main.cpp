#include <array>
#include <cstdint>
#include <samd51j20a.h>

#include <display.hpp>
#include <utility.hpp>
int main(void) {
    Display::init();
    Display::fill(0x2);

    Display::updatePixel(0x1, 0, 0);
    Display::updatePixel(0x3, 1, 1);
    Display::updatePixel(0x4, 2, 2);
    Display::updatePixel(0x5, 3, 3);

    while (true) {
        Display::draw();
    }
    return 0;
}
