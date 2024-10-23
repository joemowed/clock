#include "pio/samd51j20a.h"
#include <cstdint>
#include <samd51j20a.h>
int main() {
    PORT_REGS->GROUP[0].PORT_DIR = static_cast<uint32_t>(1 << PIN_PA00);
    PORT_REGS->GROUP[0].PORT_OUTTGL = static_cast<uint32_t>(1 << PIN_PA00);
    volatile int i = 0;
    int d = 2;
    while (true) {
        while (i < 1000) {
            i++;
            i += d;
            PORT_REGS->GROUP[0].PORT_OUTTGL = static_cast<uint32_t>(1 << PIN_PA00);
            i++;
        }
        i = 0;
    }
    return 0;
}
