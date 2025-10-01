

#include <cstdint>
#include <samd51j20a.h>
#include <setup_tc5.hpp>
// === GCLK Setup for TC5 ===
void configureGCLKForTC5(void) {
    // Enable APB clock for TC5
    MCLK_REGS->MCLK_APBCMASK |= MCLK_APBCMASK_TC5(1);

    // Configure GCLK0 (already running at 48 MHz typically)
    // Connect GCLK0 to TC5
    GCLK_REGS->GCLK_PCHCTRL[TC5_GCLK_ID] = GCLK_PCHCTRL_GEN_GCLK0 | // Use GCLK0 (48 MHz)
                                           GCLK_PCHCTRL_CHEN(1);    // Enable peripheral channel

    // Wait for sync
    while (!(GCLK_REGS->GCLK_PCHCTRL[TC5_GCLK_ID] & GCLK_PCHCTRL_CHEN(1)))
        ;
}
// === TC5 Timer Setup ===
void initTC5(void) {
    // Disable TC3 before configuration
    TC5_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while (TC5_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;

    // Reset TC3
    TC5_REGS->COUNT8.TC_CTRLA |= TC_CTRLA_SWRST_Msk;
    while (TC5_REGS->COUNT8.TC_SYNCBUSY & TC_SYNCBUSY_SWRST_Msk)
        ;

    // Configure as 8-bit timer, match frequency, prescaler div 1024, use GCLK
    TC5_REGS->COUNT8.TC_CTRLA = TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV1;
    // TC5_REGS->COUNT8.TC_CC[0] = 0xA0;

    TC5_REGS->COUNT8.TC_CC[1] = 0xFF - (0xFF * brightness);
    TC5_REGS->COUNT8.TC_WAVE = TC_WAVE_WAVEGEN_NPWM;

    while (TC5_REGS->COUNT8.TC_SYNCBUSY & TC_SYNCBUSY_CC0_Msk)
        ;

    // // Enable overflow interrupt
    // TC3_REGS->COUNT16.TC_INTENSET = TC_INTENSET_MC0_Msk;
    //
    // // Enable NVIC for TC3
    // NVIC_EnableIRQ(TC3_IRQn);

    // Enable TC5
    TC5_REGS->COUNT8.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while (TC5_REGS->COUNT8.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
}
