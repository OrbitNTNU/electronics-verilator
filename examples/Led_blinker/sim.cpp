#include "VLed_blinker.h"
#include "verilated.h"

// Needed for waveform generation
#include <verilated_vcd_c.h>

#include <cassert>
#include <vector>
#include <iostream>

void toggle_clk(VLed_blinker* top, VerilatedVcdC *trace, uint32_t *tickcount) {
    top->i_clock = 0;
    top->eval();
    trace->dump(10 * (*tickcount)++);
    top->i_clock = 1;
    top->eval();
    trace->dump(10 * (*tickcount)++);
}

void print_top(VLed_blinker *top) {
    printf("i_enable: %i\n", top->i_enable);
    printf("i_clock: %i\n", top->i_clock);
    printf("i_switch_1: %i\n", top->i_switch_1);
    printf("i_switch_2: %i\n", top->i_switch_2);
    printf("o_led_drive: %i\n", top->o_led_drive);
}

int main(int argc, char** argv) {
    VLed_blinker* top = new VLed_blinker;
    Verilated::commandArgs(argc, argv);

    // For waveform generation
    uint32_t tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");
    
    // Set initial values
    top->i_enable = 1;
    top->i_reset = 0;
    top->i_clock = 0;
    top->i_switch_1 = 0;
    top->i_switch_2 = 0;
    top->eval();

    int ret = 0;
    uint32_t count_limits[] = {
        125,
        250,
        1250,
        12500
    };
    uint32_t count = 0;

    for (int i = 0; i < sizeof(count_limits) / sizeof(count_limits[0]); i++) {
        if (Verilated::gotFinish()) break;

        top->i_switch_1 = (i & 0b01) ? 1 : 0;
        top->i_switch_2 = (i & 0b10) ? 1 : 0;
        top->i_reset = 0;

        uint32_t count_limit = count_limits[i];
        bool led_signal_expect = false;
        for (int j = 0; j < 2*count_limit; j++) {
            if (count == count_limit) {
                led_signal_expect = true;
                printf("Expect led signal high\n");
            } else if (count == 2*count_limit) {
                led_signal_expect = false;
                printf("Expect led signal  low\n");
            }
            
            if (top->o_led_drive != led_signal_expect) {
                printf("top->o_led_drive: Got %i when %i expected\n", top->o_led_drive, led_signal_expect);
                toggle_clk(top, trace, &tickcount);
                ret = 1;
                goto out;
            }

            toggle_clk(top, trace, &tickcount);
            printf("\nit: %i\n", j);
            print_top(top);
            fflush(stdout);
            count++;
        }

        printf("Toggled clock %i times. Reset\n", 2*count);
        fflush(stdout);
    }

out:
    trace->close();
    delete trace;
    
    delete top;
    return ret;
}
