#include "font.hpp"
#include <array>
std::array<Font::DecompChar, 256> Font::decompressed_font_array = {};
void Font::init() {
    for (int curr_char = '!'; curr_char < 256; curr_char++) {
        decompressed_font_array.at(curr_char) = decompress(curr_char);
    }
}
Font::DecompChar Font::decompress(uint8_t character) {
    DecompChar ret = {{}};
    const int row_offset = (character) * 5;
    for (int curr_col = 0; curr_col < 5; curr_col++) {
        const uint8_t compressed_col = COMPRESSED_FONT_ARRAY.at(row_offset + curr_col);

        for (int curr_row = 0; curr_row < 7; curr_row++) {

            volatile auto temp = ret.at(curr_row);
            temp |= compressed_col & (1 << curr_col);

            ret.at(curr_row) = temp;
        }
    }
    return ret;
}
Font::DecompChar Font::getChar(uint8_t chararcter) { return decompressed_font_array.at(chararcter); }
