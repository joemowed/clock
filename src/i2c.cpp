#include <i2c.hpp>
#include <utility.hpp>
using util::i2c_regs;
void I2C::gclk1Init(void) {
    // Disable GCLK1 before configuring
    GCLK_REGS->GCLK_GENCTRL[1] = 0; // Clear previous config
    while (GCLK_REGS->GCLK_SYNCBUSY)
        ; // Wait for sync

    // Set Division factor: 48 MHz / 4 = 12 MHz
    GCLK_REGS->GCLK_GENCTRL[1] = GCLK_GENCTRL_DIV(16) |  // Divide input by 4
                                 GCLK_GENCTRL_SRC_DFLL | // Source = DFLL (48 MHz)
                                 GCLK_GENCTRL_GENEN(1);  // Enable generator

    // Wait for synchronization
    while (GCLK_REGS->GCLK_SYNCBUSY)
        ;
}
void I2C::init(void) {
    gclk1Init();
    // 1. Enable SERCOM1 clock in APB
    MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_SERCOM1(1);

    // 2. Configure GCLK for SERCOM1
    // Assume GCLK0 = 48 MHz has already been configured
    GCLK_REGS->GCLK_PCHCTRL[SERCOM1_GCLK_ID_CORE] = GCLK_PCHCTRL_GEN_GCLK1 | GCLK_PCHCTRL_CHEN(1);
    // 3. Configure SDA and SCL pins
    // Assume PA16 -> SDA (PAD[0]), PA17 -> SCL (PAD[1])
    // Configure for peripheral function C (SERCOM1)
    util::port_a.PORT_PINCFG[16] = PORT_PINCFG_PMUXEN(1);
    util::port_a.PORT_PINCFG[17] = PORT_PINCFG_PMUXEN(1);
    util::port_a.PORT_PMUX[16 >> 1] |= PORT_PMUX_PMUXE_C; // PA16
    util::port_a.PORT_PMUX[17 >> 1] |= PORT_PMUX_PMUXO_C; // PA17
    // 4. Reset SERCOM1
    i2c_regs.SERCOM_CTRLA = SERCOM_I2CM_CTRLA_SWRST(1);
    waitForSync();
    // 5. Configure SERCOM1 for I2C master
    i2c_regs.SERCOM_CTRLA =
        SERCOM_I2CM_CTRLA_MODE_I2C_MASTER | SERCOM_I2CM_CTRLA_SPEED_STANDARD_AND_FAST_MODE | SERCOM_I2CM_CTRLA_SDAHOLD(3); // Hold time
                                                                                                                           //        SERCOM_I2CM_CTRLA_PINMUX(0);      // SDA = PAD0, SCL = PAD1
    // 6. Set baud rate for ~50 kHz
    i2c_regs.SERCOM_BAUD = SERCOM_I2CM_BAUD_BAUD(I2C::BAUD);
    waitForSync();

    // 7. Enable Smart Mode (optional but useful)
    i2c_regs.SERCOM_CTRLB = SERCOM_I2CM_CTRLB_SMEN(1) | SERCOM_I2CM_CTRLB_ACKACT(1);
    waitForSync();

    // 8. Enable SERCOM1 I2C Master
    i2c_regs.SERCOM_CTRLA |= SERCOM_I2CM_CTRLA_ENABLE(1);

    waitForSync();
    // 9. Send Bus State to IDLE
    i2c_regs.SERCOM_STATUS |= SERCOM_I2CM_STATUS_BUSSTATE(1); // Idle
    waitForSync();
}

void I2C::writeAddress(bool read) {
    const uint8_t addr = (ADDRESS << 1) | read;
    i2c_regs.SERCOM_ADDR = addr;
    waitForSync();
}

void I2C::writeAddressRead() { writeAddress(true); }

void I2C::writeAddressWrite() { writeAddress(false); }
void I2C::waitForSync() {
    while (i2c_regs.SERCOM_SYNCBUSY)
        ;
}
void I2C::stop() {
    i2c_regs.SERCOM_CTRLB |= SERCOM_I2CM_CTRLB_CMD(0x3);
    waitForSync();
}
void I2C::send(const uint8_t value) {
    waitForMB(); // will halt if an address packet with master write mode was not previously transmitted successfully
    if (SERCOM_I2CM_STATUS_RXNACK(1) & i2c_regs.SERCOM_STATUS) {
        while (true) // halt if NACK received from last byte transmitted.  This should not occur if i2c protocall followed
            ;
    }
    i2c_regs.SERCOM_DATA = value; // start transmission
    waitForSync();
}
uint8_t I2C::read(const uint8_t addr) {
    writeAddressWrite();                      // start transaction
    waitForMB();                              // wait for ACK from slave
    I2C::send(addr);                          // send address to slave
    waitForMB();                              // wait for ACK from slave
    writeAddressRead();                       // tell slave to send data
    waitForSB();                              // wait for data to be received
    const uint8_t ret = i2c_regs.SERCOM_DATA; // read the data, automatically sends NACK bit
    stop();                                   // revert bus to idle state
    return ret;
}
void I2C::write(const uint8_t addr, const uint8_t value) {

    writeAddressWrite(); // start transaction
    waitForMB();         // wait for ACK from slave
    I2C::send(addr);     // send address to slave
    waitForMB();         // wait for ACK from slave
    I2C::send(value);    // send value to slave
    waitForMB();         // wait for ACK from slave
    stop();              // revert bus to idle state
}
void I2C::waitForMB() {
    while (!util::checkBit(i2c_regs.SERCOM_INTFLAG, SERCOM_I2CM_INTFLAG_MB_Msk))
        ;
}
void I2C::waitForSB() {
    while (!util::checkBit(i2c_regs.SERCOM_INTFLAG, SERCOM_I2CM_INTFLAG_SB_Msk))
        ;
}
