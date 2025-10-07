#include "font.hpp"
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
