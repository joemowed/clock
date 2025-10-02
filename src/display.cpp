#include <display.hpp>
#include <setup_tc3.hpp>
#include <setup_tc5.hpp>
#include <utility.hpp>

Display::BitMap Display::disp_buffer;
float Display::brightness = 0.05F;

void Display::init_port_b() {
    util::port_b.PORT_DIRSET = clock_pin;
    util::port_b.PORT_DIRSET = OE_pin;
    util::port_b.PORT_DIRSET = latch_pin;
    util::port_b.PORT_OUTCLR = OE_pin;
    util::port_b.PORT_PINCFG[11] = PORT_PINCFG_PMUXEN(1);
    util::port_b.PORT_PMUX[5] = PORT_PMUX_PMUXO_E;
}

void Display::init_port_a() {
    uint32_t reg = 0;
    for (auto pin : port_a_pins) {
        reg |= pin;
    }

    util::port_a.PORT_DIR = reg;
}

uint8_t Display::validate_color(uint8_t color) {
    if (color > 0b111) {
        // max 3 bit color
        color = 0b111;
    }
    return color;
}
void Display::init() {

    configureGCLKForTC3();
    initTC3();
    configureGCLKForTC5();
    initTC5();
    init_port_a();
    init_port_b();
}
void Display::drawLines(uint8_t line_num) {

    write_address(line_num);
    const Line &upper_line = disp_buffer.at(line_num);
    const Line &lower_line = disp_buffer.at(line_num + 16);
    for (int i = 0; i < line_size; i++) {
        uint8_t upper_color = upper_line.at(i);
        uint8_t lower_color = lower_line.at(i);
        drawPixels(upper_color, lower_color);
    }
    latch();
    util::pulse_delay_loop();
}
void Display::drawPixels(uint8_t upper_color, uint8_t lower_color) {
    lower_color = validate_color(lower_color);
    lower_color = validate_color(lower_color);
    auto upper_bits = util::generate_reg_bits3(upper_color, 0);
    auto lower_bits = util::generate_reg_bits3(lower_color, 3);
    util::RegBits combined_bits;
    combined_bits.clear = upper_bits.clear | lower_bits.clear;
    combined_bits.set = upper_bits.set | lower_bits.set;

    util::write_bits(util::port_a, combined_bits);
    clock();
}
void Display::draw() {
    uint8_t curr_line_num = 0;
    constexpr const uint8_t upper_line_count = line_count / 2;
    while (curr_line_num < upper_line_count) {
        Display::drawLines(curr_line_num++);
    }
}
void Display::updatePixel(uint8_t color, uint8_t pixel_pos, uint8_t line_num) {
    color = validate_color(color);
    pixel_pos = (pixel_pos >= line_size) ? 0 : pixel_pos;
    line_num = (line_num >= line_count) ? 0 : line_num;
    Display::disp_buffer.at(line_num).at(pixel_pos) = color;
}

void Display::fill(uint8_t color) {
    for (int line_num = 0; line_num < line_count; line_num++) {
        for (int line_pos = 0; line_pos < line_size; line_pos++) {
            updatePixel(color, line_pos, line_num);
        }
    }
}
void Display::clock() {
    stopPWM();
    pulse_port_b(clock_pin);
    startPWM();
}
void Display::latch() { pulse_port_b(latch_pin); }
void Display::pulse_port_b(const uint32_t pin) {
    util::pulse_delay_loop();
    util::port_b.PORT_OUTSET = pin;
    util::pulse_delay_loop();
    util::port_b.PORT_OUTCLR = pin;
}

void Display::write_address(uint8_t addr) {
    // clamp addr to 4 bit max
    if (addr > 0xF) {
        addr = 0xF;
    }
    auto bits = util::generate_reg_bits4(addr, 6);
    util::write_bits(util::port_a, bits);
}
