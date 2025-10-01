#include <array>
#include <cstdint>
#include <samd51j20a.h>
class Display {

  public:
    static constexpr const uint8_t line_size = 64;
    static constexpr const uint8_t line_count = 16;
    static constexpr const uint8_t color_bits = 3;

    using Line = std::array<uint8_t, line_size>;
    using BitMap = std::array<Line, line_count>;
    Display() = delete;
    Display(Display &) = delete;
    Display(Display &&) = delete;
    ~Display() = delete;
    static void init();
    static void updatePixel(uint8_t color, uint8_t line_pos, uint8_t line_num);
    static void draw();
    static void fill(uint8_t color);

  private:
    static BitMap disp_buffer;
    static void select_line(uint8_t line_num);
    static void init_port_a();
    static void init_port_b();
    static uint8_t validate_color(uint8_t color);
    static void drawLine(Line line, uint8_t line_num);
    static void drawPixel(uint8_t color);
    static const uint32_t clock_pin = PORT_PB10;
    static const uint32_t OE_pin = PORT_PB11;
    static const uint32_t latch_pin = PORT_PB12;
    constexpr static const std::array<uint32_t, 10> port_a_pins = {PORT_PA00, PORT_PA01, PORT_PA02, PORT_PA03, PORT_PA04, PORT_PA05, PORT_PA06, PORT_PA07, PORT_PA08, PORT_PA09};
};
