#include <display.hpp>
#include <setup_tc3.hpp>
#include <setup_tc5.hpp>
#include <utility.hpp>

Display::BitMap Display::disp_buffer;

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
    if (color > color_bits) {
        // max 3 bit color
        color = 0xFF;
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
void Display::drawLine(Line line, uint8_t line_num) {
    for (const uint8_t &curr_pixel_color : line) {
        drawPixel(curr_pixel_color);
    }
}
drawPixel(uint8_t color) { color = validate_color }
void Display::draw() {
    uint8_t curr_line_num = 0;
    for (const Display::Line &curr_line : disp_buffer) {
        Display::drawLine(curr_line, curr_line_num++);
    }
}
void Display::updatePixel(uint8_t color, uint8_t line_pos, uint8_t line_num) {
    color = validate_color(color);
    line_pos = (line_pos >= line_size) ? 0 : line_pos;
    line_num = (line_num >= line_count) ? 0 : line_num;
    Display::disp_buffer.at(line_num).at(line_pos) = color;
}

void Display::fill(uint8_t color) {
    for (int line_num = 0; line_num < line_count; line_num++) {
        for (int line_pos = 0; line_pos < line_size; line_pos++) {
            updatePixel(color, line_pos, line_num);
        }
    }
}
