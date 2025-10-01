#include <array>
#include <cstdint>
#include <samd51j20a.h>

#include <display.hpp>
#include <utility.hpp>
const uint32_t clock_pin = PORT_PB10;
const uint32_t OE_pin = PORT_PB11;
const uint32_t latch_pin = PORT_PB12;
std::array<uint32_t, 10> port_a_pins = {PORT_PA00, PORT_PA01, PORT_PA02, PORT_PA03, PORT_PA04, PORT_PA05, PORT_PA06, PORT_PA07, PORT_PA08, PORT_PA09};
typedef struct RegBits {
    uint32_t set;
    uint32_t clear;
} RegBits;
RegBits generate_reg_bits(uint32_t value, uint32_t offset, uint32_t length) {

    uint32_t generated_msk = 0;
    for (uint32_t i = 0; i < length; i++) {
        generated_msk |= 1 << i;
    }

    const uint32_t address_msk = (generated_msk << offset);

    const uint32_t set_bits = value << offset;
    const uint32_t clear_bits = (~set_bits) & address_msk;
    return {set_bits, clear_bits};
}
void write_bits(port_group_registers_t &port, const RegBits bits) {
    port.PORT_OUTSET = bits.set;
    port.PORT_OUTCLR = bits.clear;
}
void write_address(uint8_t addr) {
    // clamp addr to 4 bit max
    if (addr > 0xF) {
        addr = 0xF;
    }
    auto bits = generate_reg_bits(addr, 6, 4);
    write_bits(port_a, bits);
}
uint32_t validate_color(uint8_t color, const uint8_t offset) {
    if (color > 0b111) {
        // max 3 bit color
        color = 0b111;
    }
    return color;
}
void write_color0(uint8_t color) {
    color = validate_color(color, 0);
    auto bits = generate_reg_bits(color, 0, 3);
    write_bits(port_a, bits);
}
void write_color1(uint8_t color);
void delay_loop() {

    const uint32_t clock_loop_max = 0x02;
    for (volatile uint32_t i = 0; i < clock_loop_max; i += 1)
        ;
}
void pulse_port_b(const uint32_t pin) {
    delay_loop();
    port_b.PORT_OUTSET = pin;
    delay_loop();
    port_b.PORT_OUTCLR = pin;
}
// === Timer Interrupt Handler ===
int main(void) {
    Display::init();
    for (auto each : port_a_pins) {
        port_a.PORT_OUTSET = each;
    }
    while (true) {
    }
    return 0;
}
