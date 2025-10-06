#include <array>
#include <cstdint>
#include <louie.hpp>
class Transcode {
  public:
    static constexpr const uint8_t RED_Pos = 0;
    static constexpr const uint8_t BLUE_Pos = 1;
    static constexpr const uint8_t GREEN_Pos = 2;
    Transcode() = delete;
    Transcode(Transcode &) = delete;
    Transcode(Transcode &&) = delete;
    ~Transcode() = delete;
    static std::array<uint8_t, LOUIE_PIXEL_COUNT> transcodeLouie();

  private:
    typedef struct Bits {
        bool red;
        bool blue;
        bool green;
    } Bits;
    static Bits convertToBits(uint16_t color);
    static uint8_t convertTo8Bit(Bits color);
    static uint8_t RGB565ToRGB3(uint16_t color);
};
