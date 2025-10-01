#include <display.hpp>
#include <setup_tc3.hpp>
#include <setup_tc5.hpp>
#include <utility.hpp>
void Display::init_port_b() {
    port_b.PORT_DIRSET = clock_pin;
    port_b.PORT_DIRSET = OE_pin;
    port_b.PORT_DIRSET = latch_pin;
    port_b.PORT_OUTCLR = OE_pin;
    port_b.PORT_PINCFG[11] = PORT_PINCFG_PMUXEN(1);
    port_b.PORT_PMUX[5] = PORT_PMUX_PMUXO_E;
}
void Display::init_port_a() {
    uint32_t reg = 0;
    for (auto pin : port_a_pins) {
        reg |= pin;
    }

    port_a.PORT_DIR = reg;
}

void Display::init() {

    configureGCLKForTC3();
    initTC3();
    configureGCLKForTC5();
    initTC5();
    init_port_a();
    init_port_b();
}
void Display::drawLine(Line line) {}
void Display::draw() {
    for (const Display::Line &curr_line : disp_buffer) {
        Display::drawLine(curr_line);
    }
}
