#include "samd51j20a.h"
#include <cstdint>
class I2C {
    constexpr static uint8_t BAUD = 150;
    constexpr static uint8_t ADDRESS = 0x6F; // 7 bits

  public:
    I2C() = delete;
    I2C(I2C &) = delete;
    I2C(I2C &&) = delete;
    ~I2C() = delete;
    static void init();
    static uint8_t read(const uint8_t addr);
    static void write(const uint8_t addr, const uint8_t value);
    static void send(const uint8_t value);

    // private:
    static void stop();
    static void waitForSync();
    static void gclk1Init();
    static void writeAddressRead();
    static void writeAddressWrite();
    static void writeAddress(bool read);
    static void waitForMB();
    static void waitForSB();
};
