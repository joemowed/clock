#include "samd51j20a.h"
namespace util {
inline port_group_registers_t &port_a = PORT_REGS->GROUP[0];
inline port_group_registers_t &port_b = PORT_REGS->GROUP[1];

consteval inline uint32_t generateMask(const uint32_t length) {
    uint32_t generated_msk = 0;
    for (uint32_t i = 0; i < length; i++) {
        generated_msk |= 1 << i;
    }
    return generated_msk;
}

inline void pulse_delay_loop() {

    constexpr const uint32_t clock_loop_max = 0x00;
    for (volatile uint32_t i = 0; i < clock_loop_max; i += 1)
        ;
}

typedef struct RegBits {
    uint32_t set;
    uint32_t clear;
} RegBits;
inline RegBits generate_reg_bits3(const uint32_t value, const uint32_t offset) {

    uint32_t generated_msk = util::generateMask(3);

    const uint32_t address_msk = (generated_msk << offset);

    const uint32_t set_bits = value << offset;
    const uint32_t clear_bits = (~set_bits) & address_msk;
    return {set_bits, clear_bits};
}
inline RegBits generate_reg_bits4(const uint32_t value, const uint32_t offset) {

    uint32_t generated_msk = util::generateMask(4);

    const uint32_t address_msk = (generated_msk << offset);

    const uint32_t set_bits = value << offset;
    const uint32_t clear_bits = (~set_bits) & address_msk;
    return {set_bits, clear_bits};
}
inline void write_bits(port_group_registers_t &port, const RegBits bits) {
    port.PORT_OUTSET = bits.set;
    port.PORT_OUTCLR = bits.clear;
}
} // namespace util
