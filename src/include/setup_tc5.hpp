
#include <cstdint>
#include <samd51j20a.h>

// brightness is a value between 0 and 1 that is duty cycle of display
constexpr const float brightness = 0.05;

// === GCLK Setup for TC3 ===
void configureGCLKForTC5(void);
// === TC3 Timer Setup ===
void initTC5(void);
