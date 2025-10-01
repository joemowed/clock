#include <array>
#include <cstdint>
#include <samd51j20a.h>

#include <display.hpp>
#include <utility.hpp>
const uint32_t clock_pin = PORT_PB10;
const uint32_t OE_pin = PORT_PB11;
const uint32_t latch_pin = PORT_PB12;
std::array<uint32_t, 10> port_a_pins = {PORT_PA00, PORT_PA01, PORT_PA02, PORT_PA03, PORT_PA04, PORT_PA05, PORT_PA06, PORT_PA07, PORT_PA08, PORT_PA09};
void write_bits(port_group_registers_t &port, const util::RegBits bits) {
    port.PORT_OUTSET = bits.set;
    port.PORT_OUTCLR = bits.clear;
}
void write_address(uint8_t addr) {
    // clamp addr to 4 bit max
    if (addr > 0xF) {
        addr = 0xF;
    }
    auto bits = util::generate_reg_bits(addr, 6, 4);
    write_bits(util::port_a, bits);
}
void write_color0(uint8_t color) {
    color = validate_color(color);
    auto bits = util::generate_reg_bits(color, 0, 3);
    write_bits(util::port_a, bits);
}
void write_color1(uint8_t color);
void pulse_port_b(const uint32_t pin) {
    util::pulse_delay_loop();
    util::port_b.PORT_OUTSET = pin;
    util::pulse_delay_loop();
    util::port_b.PORT_OUTCLR = pin;
}
// === Timer Interrupt Handler ===
int main(void) {
    Display::init();
    for (auto each : port_a_pins) {
        util::port_a.PORT_OUTSET = each;
    }
    while (true) {
    }
    return 0;
}
