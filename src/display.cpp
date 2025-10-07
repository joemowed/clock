#include <display.hpp>
#include <setup_tc3.hpp>
#include <setup_tc5.hpp>
#include <utility.hpp>

Display::BitMap Display::disp_buffer;
uint8_t Display::brightness = 30;
uint8_t Display::curr_draw_line_num = 0;

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
    reg |= PORT_PA15;
    util::port_a.PORT_DIR = reg;
}

uint8_t Display::validateColor(uint8_t color) {
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
    outputOff();
    writeAddress(line_num);
    Line &upper_line = disp_buffer.at(line_num + 16);
    Line &lower_line = disp_buffer.at(line_num);

    for (int i = 0; i < line_size; i++) {
        uint8_t upper_color = upper_line.at(i);
        uint8_t lower_color = lower_line.at(i);
        lower_color = (lower_color > 0b111) ? 0b111 : lower_color;
        upper_color = (upper_color > 0b111) ? 0b111 : upper_color;
        uint32_t port_a_state = util::port_a.PORT_OUT;
        port_a_state &= ~0b111111; // clear color bits
        port_a_state |= ((upper_color << 3) | lower_color);
        util::port_a.PORT_OUT = port_a_state;
        clock();
    }
    latch();
    outputOn();
}
inline void Display::drawPixels(uint8_t upper_color, uint8_t lower_color) {
    lower_color = (lower_color > 0b111) ? 0b111 : lower_color;
    upper_color = (upper_color > 0b111) ? 0b111 : upper_color;
    // auto upper_bits = util::generate_reg_bits3(upper_color, 0);
    // auto lower_bits = util::generate_reg_bits3(lower_color, 3);
    // util::RegBits combined_bits;
    // combined_bits.clear = upper_bits.clear | lower_bits.clear;
    // combined_bits.set = upper_bits.set | lower_bits.set;
    // util::write_bits(util::port_a, combined_bits);
    uint32_t port_a_state = util::port_a.PORT_OUT;
    port_a_state &= ~0b111111; // clear color bits
    port_a_state |= ((upper_color << 3) | lower_color);
    util::port_a.PORT_OUT = port_a_state;
    clock();
}
void Display::draw() {
    curr_draw_line_num = (curr_draw_line_num >= line_count / 2) ? 0 : curr_draw_line_num;
    Display::drawLines(curr_draw_line_num++);
}
void Display::updatePixel(uint8_t color, uint8_t pixel_pos, uint8_t line_num) {
    color = validateColor(color);
    pixel_pos = (pixel_pos >= line_size) ? 0 : pixel_pos;
    line_num = (line_num >= line_count) ? 0 : line_num;
    if (pixel_pos >= line_size || line_num >= line_count) {
        color = 0x0;
    }
    Display::disp_buffer.at(line_num).at(pixel_pos) = color;
}

void Display::fill(uint8_t color) {
    for (int line_num = 0; line_num < line_count; line_num++) {
        for (int line_pos = 0; line_pos < line_size; line_pos++) {
            updatePixel(color, line_pos, line_num);
        }
    }
}
void Display::outputOff() {

    util::port_b.PORT_PINCFG[11] = PORT_PINCFG_PMUXEN(0);
    util::port_b.PORT_OUTSET = OE_pin;
}
void Display::outputOn() { util::port_b.PORT_PINCFG[11] = PORT_PINCFG_PMUXEN(1); }
void Display::clock() { pulsePortB(clock_pin); }
void Display::latch() { pulsePortB(latch_pin); }
void delayLoop() {
    for (volatile int i = 0; i < 1; i += 0xFFFF) {
    }
}
void Display::pulsePortB(const uint32_t pin) {
    // delayLoop();
    util::port_b.PORT_OUTSET = pin;
    // delayLoop();
    util::port_b.PORT_OUTCLR = pin;
}

void Display::writeAddress(uint8_t addr) {

    // clamp addr to 4 bit max
    if (addr > 0xF) {
        addr = 0xF;
    }
    constexpr const uint32_t addr_msk = 0b1111 << 6;
    util::port_a.PORT_OUTCLR = addr_msk;
    util::port_a.PORT_OUTSET = addr << 6;
}
