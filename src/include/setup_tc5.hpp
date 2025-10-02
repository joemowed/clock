
#include <cstdint>
#include <samd51j20a.h>

// === GCLK Setup for TC3 ===
void configureGCLKForTC5(void);
// === TC3 Timer Setup ===
void initTC5(void);
void startPWM();
void stopPWM();
