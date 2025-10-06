#include <array>
#include <cstdint>
#include <samd51j20a.h>

#include <display.hpp>
#include <utility.hpp>
int main(void) {
    Display::init();
    Display::fill(0x7);
    // Display::updatePixel(0x6, 0, 0);
    // Display::updatePixel(0x6, 4, 4);
    // Display::updatePixel(0x6, 16, 16);
    while (true) {
    }
    return 0;
}
