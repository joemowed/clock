#include <cstdint>
#include <samd51j20a.h>

#define TIMER_TICKS_PER_SEC 1000 // 1ms tick
#define PRESCALER_DIV 1024
#define TARGET_TICKS 1000 // 1000 ms = 1 second

volatile uint32_t timer_ticks = 0;

// === GCLK Setup for TC3 ===
void configure_gclk_for_tc3(void) {
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
void configure_tc3_timer(void) {
    // Disable TC3 before configuration
    TC3_REGS->COUNT16.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    while (TC3_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;

    // Reset TC3
    TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_SWRST_Msk;
    while (TC3_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_SWRST_Msk)
        ;

    // Configure as 16-bit timer, match frequency, prescaler div 1024, use GCLK
    TC3_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV256;
    TC3_REGS->COUNT16.TC_CC[0] = 0x1;

    while (TC3_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_CC0_Msk)
        ;

    // Enable overflow interrupt
    TC3_REGS->COUNT16.TC_INTENSET = TC_INTENSET_MC0_Msk;

    // Enable NVIC for TC3
    NVIC_EnableIRQ(TC3_IRQn);

    // Enable TC3
    TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    while (TC3_REGS->COUNT16.TC_SYNCBUSY & TC_SYNCBUSY_ENABLE_Msk)
        ;
}

// === Timer Interrupt Handler ===
void TC3_Handler(void) {
    if (TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC0_Msk) {
        TC3_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_MC0_Msk; // Clear interrupt

        PORT_REGS->GROUP[0].PORT_OUTTGL = static_cast<uint32_t>(1 << PIN_PA00);
    }
}

int main(void) {
    configure_gclk_for_tc3();
    configure_tc3_timer();
    PORT_REGS->GROUP[0].PORT_DIR = static_cast<uint32_t>(1 << PIN_PA00);
    PORT_REGS->GROUP[0].PORT_OUTTGL = static_cast<uint32_t>(1 << PIN_PA00);
    while (true) {
    }
    return 0;
}
