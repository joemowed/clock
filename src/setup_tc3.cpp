

#include <cstdint>
#include <display.hpp>
#include <samd51j20a.h>
// === GCLK Setup for TC3 ===
void configureGCLKForTC3(void) {
    // Enable APB clock for TC3
    MCLK_REGS->MCLK_APBBMASK |= MCLK_APBBMASK_TC3(1);
    // Configure GCLK0 (already running at 48 MHz typically)
    // Connect GCLK0 to TC3
    GCLK_REGS->GCLK_PCHCTRL[TC3_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK0 | // Use GCLK0 (48 MHz)
                                           GCLK_PCHCTRL_CHEN(1);    // Enable peripheral channel

    // Wait for sync
    while (!(GCLK_REGS->GCLK_PCHCTRL[TC3_GCLK_ID] & GCLK_PCHCTRL_CHEN(1)))
        ;
}

// === TC3 Timer Setup ===
void initTC3(void) {
    // Disable TC3 before configuration
    TC3_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while (TC3_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;

    // Reset TC3
    TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_SWRST_Msk;
    while (TC3_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_SWRST_Msk)
        ;

    TC3_REGS->COUNT8.TC_CTRLA = TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV1024;
    TC3_REGS->COUNT8.TC_CC[0] = 0x4;

    while (TC3_REGS->COUNT8.TC_SYNCBUSY & TC_SYNCBUSY_CC0_Msk)
        ;

    // Enable overflow interrupt
    TC3_REGS->COUNT8.TC_INTENSET = TC_INTENSET_MC0_Msk;

    // Enable NVIC for TC3
    NVIC_EnableIRQ(TC3_IRQn);

    // Enable TC3
    TC3_REGS->COUNT8.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while (TC3_REGS->COUNT8.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
}
#include "utility.hpp"
void TC3_Handler(void) {
    // Disable TC3
    TC3_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while (TC3_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
    TC3_REGS->COUNT8.TC_COUNT = 0; // Reset counter to zero
    while (TC3_REGS->COUNT8.TC_SYNCBUSY)
        ; // Wait for sync
    Display::draw();

    if (TC3_REGS->COUNT8.TC_INTFLAG & TC_INTFLAG_MC0(1)) {
        TC3_REGS->COUNT8.TC_INTFLAG = TC_INTFLAG_MC0(1); // Clear interrupt
    }
    // Enable TC3
    TC3_REGS->COUNT8.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while (TC3_REGS->COUNT8.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
}
