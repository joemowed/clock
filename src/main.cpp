#include <array>
#include <cstdint>
#include <samd51j20a.h>

#include <setup_tc3.hpp>
#include <setup_tc5.hpp>
auto &port_a = PORT_REGS->GROUP[0];
auto &port_b = PORT_REGS->GROUP[1];

const auto &clock_pin = PORT_PB10;
const auto &OE_pin = PORT_PB11;
const auto &latch_pin = PORT_PB12;

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
void TC3_Handler(void) {
    port_a.PORT_OUTCLR = port_a_pins[3];
    port_a.PORT_OUTSET = port_a_pins[3];
    static uint8_t curr_addr = 0;
    static uint8_t color0 = 0;
    if (TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC0_Msk) {
        TC3_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_MC0_Msk; // Clear interrupt
    }
    write_address(curr_addr++);
    curr_addr = (curr_addr > 0xF) ? 0x0 : curr_addr;
    write_color0(color0++);
    pulse_port_b(0x0);
    color0 = (color0 > 0b111) ? 0x0 : color0;
    pulse_port_b(clock_pin);
    pulse_port_b(latch_pin);
    port_a.PORT_OUTCLR = port_a_pins[3];
}
void init_port_b() {
    port_b.PORT_DIRSET = clock_pin;
    port_b.PORT_DIRSET = OE_pin;
    port_b.PORT_DIRSET = latch_pin;
    port_b.PORT_OUTCLR = OE_pin;
    port_b.PORT_PINCFG[11] = PORT_PINCFG_PMUXEN(1);
    port_b.PORT_PMUX[5] = PORT_PMUX_PMUXO_E;
}
void init_port_a() {
    uint32_t reg = 0;
    for (auto pin : port_a_pins) {
        reg |= pin;
    }

    port_a.PORT_DIR = reg;
}
int main(void) {
    configureGCLKForTC3();
    initTC3();
    configureGCLKForTC5();
    initTC5();
    init_port_a();
    init_port_b();
    for (auto each : port_a_pins) {
        port_a.PORT_OUTSET = each;
    }
    while (true) {
    }
    return 0;
}
