#include "VUART_rx.h"
#include "verilated.h"

// Needed for waveform generation
#include <verilated_vcd_c.h>

#include <cassert>
#include <vector>
#include <iostream>

#define CLK_FREQ 100000000
#define BAUDRATE 115200
#define CLKS_PER_BAUD (CLK_FREQ / BAUDRATE)

void apply_clock_cycle(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount) {
    top->clk = !top->clk;
    top->eval();
    trace->dump(10 * (*tickcount)++);
    top->clk = !top->clk;
    top->eval();
}

void await_half_baud(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount) {
    // Just toggle the clock for the number of clock cycles in a single baud
    for (int i = 0; i < CLKS_PER_BAUD / 2; i++) {
        apply_clock_cycle(top, trace, tickcount);
    }
}

void await_one_baud(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount) {
    await_half_baud(top, trace, tickcount);
    await_half_baud(top, trace, tickcount);
}

void stimulate_bit(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount, bool bit) {
    top->rx_line = bit;
    await_one_baud(top, trace, tickcount);
}

void stimulate_byte(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount, uint8_t byte, bool stopbit) {
    // Start bit
    stimulate_bit(top, trace, tickcount, 0);
    if (top->enable) {
        assert(!top->rx_byte_valid);
    }
    
    for (int i = 0; i < 8; i++) {
        // Data bits
        stimulate_bit(top, trace, tickcount, byte & (1 << i));
        if (top->enable) {
            assert(!top->rx_byte_valid);
        }
    }

    // Stop bit (it is an input to the function so it can be used to test
    // error on stop bit as well)
    stimulate_bit(top, trace, tickcount, stopbit);
}

void stimulate_and_check_byte(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount, uint8_t byte) {
    stimulate_byte(top, trace, tickcount, byte, 1);
    
    // Synchronize with the digital circuit
    while (!top->rx_byte_valid) {
        apply_clock_cycle(top, trace, tickcount);
        assert(!top->rx_byte_error);
    }
    assert(top->rdy);
    assert(!top->rx_byte_error);
    assert(top->rx_byte == byte);
}

void test_disabled_circuit(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount, uint8_t byte) {
    
    // Set enable low and expect no changes on outputs despite changes on inputs
    top->enable = false;
    bool rdy_before = top->rdy;
    bool rx_byte_valid_before = top->rx_byte_valid;
    bool rx_byte_error_before = top->rx_byte_error;
    uint8_t rx_byte_before = top->rx_byte;

    stimulate_byte(top, trace, tickcount, byte, 1);

    assert(rdy_before == top->rdy);
    assert(rx_byte_valid_before == top->rx_byte_valid);
    assert(rx_byte_error_before == top->rx_byte_error);
    assert(rx_byte_before == top->rx_byte);
}

void test_reset(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount) {

    // Reset circuit and check its initial values
    top->reset = true;
    
    // Reset happens on rising edge of clock; we don't assume anything about what
    // the clock is so we just toggle it twice to make sure we get a rising edge
    apply_clock_cycle(top, trace, tickcount);

    assert(top->rdy == false);
    assert(top->rx_byte_valid == false);
    assert(top->rx_byte_error == false);
    assert(top->rx_byte == 0x00);
}

void test_error_data(VUART_rx *top, VerilatedVcdC *trace, uint64_t *tickcount, uint8_t byte) {
    stimulate_byte(top, trace, tickcount, byte, 0);
    assert(!top->rx_byte_valid);
    assert(top->rx_byte_error);

    // Await some time and check that the error is still there
    await_half_baud(top, trace, tickcount);
    assert(!top->rx_byte_valid);
    assert(top->rx_byte_error);

    // The error disappears when the rx line is high again
    top->rx_line = true;
    apply_clock_cycle(top, trace, tickcount);

    // The byte shalll be invalid and the error shall still be there until the
    // next byte is started
    assert(!top->rx_byte_valid);
    assert(top->rx_byte_error);
}

int main(int argc, char** argv) {
    VUART_rx* top = new VUART_rx;
    Verilated::commandArgs(argc, argv);

    // For waveform generation
    uint64_t tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");

    // Reset circuit (should always be done before use)
    top->reset = true;
    apply_clock_cycle(top, trace, &tickcount);

    // Initialize all inputs
    top->clk = false;
    top->enable = true;
    top->reset = false;
    top->rx_line = true;
    top->eval();

    // Test simple byte receive
    stimulate_and_check_byte(top, trace, &tickcount, 0xAA);
    stimulate_and_check_byte(top, trace, &tickcount, 0x55);

    // Test that nothing happens on outputs when circuit is disabled
    test_disabled_circuit(top, trace, &tickcount, 0xAA);
    test_disabled_circuit(top, trace, &tickcount, 0x55);

    // Test normal reset
    test_reset(top, trace, &tickcount);

    // Check reset works despite the value of enable
    top->enable = false;
    top->reset = false;
    top->eval();

    test_reset(top, trace, &tickcount);

    // Check the UART still works as intended
    top->enable = true;
    top->reset = false;
    stimulate_and_check_byte(top, trace, &tickcount, 0xA5);
    stimulate_and_check_byte(top, trace, &tickcount, 0x5A);

    // Try to send errorneous data (i.e. without stop bit)
    test_error_data(top, trace, &tickcount, 0xAA);

    // Get UART back to normal state by setting rx line high again
    top->rx_line = true;
    apply_clock_cycle(top, trace, &tickcount);
    assert(!top->rx_byte_valid);
    assert(top->rx_byte_error); 

    // Try to receive lots of data without break
    for (int i = 0; i < 0xFF; i++) {
        stimulate_and_check_byte(top, trace, &tickcount, i);
    }

    // Dump a final time to avoid early cutoff
    trace->dump(10*tickcount++);

    trace->close();
    delete trace;

    delete top;
    return 0;
}
