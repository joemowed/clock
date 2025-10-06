#include <transcode.hpp>
std::array<uint8_t, LOUIE_PIXEL_COUNT> Transcode::transcodeLouie() {
    std::array<uint8_t, LOUIE_PIXEL_COUNT> ret = {};
    uint32_t curr_index = 0;
    for (uint16_t curr_pixel : louiev2) {
        ret.at(curr_index++) = RGB565ToRGB3(curr_pixel);
    }
    return ret;
}

// Convert 16-bit RGB565 to 3-bit RGB (1 bit per channel)
uint8_t Transcode::RGB565ToRGB3(uint16_t rgb565) {
    // Extract 5-bit red (bits 11–15), shift to MSB
    uint8_t r5 = (rgb565 >> 11) & 0x1F;
    // Extract 6-bit green (bits 5–10)
    uint8_t g6 = (rgb565 >> 5) & 0x3F;
    // Extract 5-bit blue (bits 0–4)
    uint8_t b5 = rgb565 & 0x1F;

    // Reduce to 1 bit per channel
    uint8_t r1 = (r5 > 15) ? 1 : 0; // threshold: middle of 0–31
    uint8_t g1 = (g6 > 31) ? 1 : 0; // threshold: middle of 0–63
    uint8_t b1 = (b5 > 15) ? 1 : 0; // threshold: middle of 0–31

    // Combine into 3-bit RGB
    return (r1 << RED_Pos) | (g1 << GREEN_Pos) | (b1 << BLUE_Pos);
}
// Transcode::Bits Transcode::convertToBits(uint16_t color) {
//     Bits ret = {};
//     constexpr const uint16_t BIT16_RED_Msk = 0b1111100000000000;
//     constexpr const uint16_t BIT16_GREEN_Msk = 0b0000011111100000;
//     constexpr const uint16_t BIT16_BLUE_Msk = 0b0000000000011111;
//     constexpr const uint8_t BIT16_RED_THRESHOLD = 0b1111;
//     constexpr const uint8_t BIT16_GREEN_THRESHOLD = 0b11111;
//     constexpr const uint8_t BIT16_BLUE_THRESHOLD = 0b1111;
//
//     uint16_t red_value = color &= BIT16_RED_Msk;
//     uint16_t green_value = color &= BIT16_GREEN_Msk;
//     uint16_t blue_value = color &= BIT16_BLUE_Msk;
//
//     red_value >>= 11;
//     green_value >>= 5;
//
//     ret.red = red_value >= BIT16_RED_THRESHOLD;
//     ret.green = green_value >= BIT16_GREEN_THRESHOLD;
//     ret.blue = blue_value >= BIT16_BLUE_THRESHOLD;
//
//     return ret;
// }
// uint8_t Transcode::convertTo8Bit(Bits color) {
//     uint8_t ret = 0;
//     ret |= color.red << RED_Pos;
//     ret |= color.green << GREEN_Pos;
//     ret |= color.blue << BLUE_Pos;
//     return ret;
// }
