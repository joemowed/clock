#include "font.hpp"
#include "display.hpp"
#include <array>
Font::DecompChar Font::getChar(uint8_t chararcter) {
    Font::DecompChar ret = {};
    uint32_t comp_offset = (chararcter - START_OFFSET) * 5;
    for (int x = 0; x < 5; x++) {
        const uint8_t curr_col = COMPRESSED_FONT_ARRAY.at(comp_offset + x);
        for (int y = 0; y < 8; y++) {
            uint8_t temp = curr_col & (1 << y);
            temp = (temp > 0);
            temp <<= x;
            ret.at(y) |= temp;
        }
    }
    return ret;
}
void Font::drawStr(char *str, const uint8_t len, const uint8_t color, const uint8_t x_offset, const uint8_t line) {
    uint8_t y_offset = (7 * line) + (5 * line); // 0 for line 1
    for (int i = 0; i < len; i++) {
        drawChar(str[i], color, x_offset + (i * 6), y_offset);
    }
}

void Font::drawChar(const uint8_t character, const uint8_t color, const uint8_t x_offset, const uint8_t y_offset) {

    Font::DecompChar curr_char = Font::getChar(character);
    for (int y = 0; y < 7; y++) {
        const uint8_t curr_row = curr_char.at(y);
        for (int x = 0; x < 5; x++) {
            if (curr_row & (1 << x)) {
                Display::updatePixel(color, x + x_offset, y + y_offset);

            } else {
                Display::updatePixel(0x0, x + x_offset, y + y_offset);
            }
        }
    }
}
