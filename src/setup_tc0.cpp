
#include <cstdint>
#include <display.hpp>
#include <draw.hpp>
#include <samd51j20a.h>
#include <setup_tc0.hpp>
// === GCLK Setup for TC3 ===
void configureGCLKForTC0(void) {
    // Enable APB clock for TC0
    MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_TC0(1);

    // Configure GCLK0 (already running at 48 MHz typically)
    // Connect GCLK0 to TC0
    GCLK_REGS->GCLK_PCHCTRL[TC0_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK1 | // Use GCLK1 (set very slow for i2c)
                                           GCLK_PCHCTRL_CHEN(1);    // Enable peripheral channel

    // Wait for sync
    while (!(GCLK_REGS->GCLK_PCHCTRL[TC0_GCLK_ID] & GCLK_PCHCTRL_CHEN(1)))
        ;
}
// === TC0 Timer Setup ===
void initTC0(void) {
    // Disable TC0 before configuration
    TC0_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while (TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
    // Reset TC0
    TC0_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_SWRST_Msk;
    while (TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_SWRST_Msk)
        ;

    TC0_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1024;
    TC0_REGS->COUNT16.TC_CC[0] = 0x2FF;

    while (TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_CC0_Msk)
        ;

    // Enable overflow interrupt
    TC0_REGS->COUNT16.TC_INTENSET = TC_INTENSET_MC0_Msk;

    // Enable NVIC for TC0
    NVIC_EnableIRQ(TC0_IRQn);

    // Enable TC0
    TC0_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while (TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
}
#include "utility.hpp"
void TC0_Handler(void) {
    // Disable TC0
    TC0_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while (TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
    TC0_REGS->COUNT16.TC_COUNT = 0; // Reset counter to zero
    while (TC0_REGS->COUNT16.TC_SYNCBUSY)
        ; // Wait for sync
    util::timingPulseToggle();
    Draw::updateFlag = true;
    if (TC0_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC0(1)) {
        TC0_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_MC0(1); // Clear interrupt
    }
    // Enable TC0
    TC0_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while (TC0_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
}
