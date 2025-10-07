#include <array>
#include <cstdint>
#include <display.hpp>
#include <draw.hpp>
#include <font.hpp>
#include <samd51j20a.h>
#include <transcode.hpp>
#include <utility.hpp>

void wdt_init() {
    // 1. Enable clock to WDT (on the low-power domain)
    MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_WDT(1);

    // 2. Use OSCULP32K as clock source (default for WDT)
    // Make sure GCLK1 or others are not routed to WDT (not needed unless using GCLK)

    // 3. Disable WDT before configuring (just in case)
    WDT_REGS->WDT_CTRLA = 0;
    while (WDT_REGS->WDT_SYNCBUSY)
        ;

    // 4. Set timeout period (8 = ~8 sec, depending on clock)
    // WDT uses ~1.024 kHz OSCULP32K by default
    // See datasheet Table 20-4 for period values
    WDT_REGS->WDT_CONFIG = WDT_CONFIG_PER_CYC1024;

    // WDT_REGS->WDT_INTENSET = WDT_INTENSET_EW_Msk;
    while (WDT_REGS->WDT_SYNCBUSY)
        ;

    // 5. Enable WDT
    WDT_REGS->WDT_CTRLA = WDT_CTRLA_ENABLE(1) | WDT_CTRLA_ALWAYSON(1);
    while (WDT_REGS->WDT_SYNCBUSY & WDT_SYNCBUSY_ENABLE_Msk)
        ;
}
void wdt_clear() {
    // Clear WDT to prevent reset
    WDT_REGS->WDT_CLEAR = WDT_CLEAR_CLEAR_KEY; // 0xA5
    while (WDT_REGS->WDT_SYNCBUSY & WDT_SYNCBUSY_CLEAR_Msk)
        ;
}
#include <i2c.hpp>
int main(void) {
    wdt_init();
    Display::init();
    RTC::init();

    // RTC::init(00, 9, 5, 2, 7, 10, 25, true);
    //  Font::drawStr(" THR", 6, 0x5, 3, 0);
    //  Font::drawStr("00:00", 6, 0x7, 3, 1);
    //  loadLouie(33, -2);
    //  Font::drawStr("OCT 10", 6, 0x6, 0, 2);

    while (true) {
        // util::timingPulseOn();
        Draw::update();
        wdt_clear();
        // util::timingPulseOff();
    }
    return 0;
}
