#include "VStates.h"
#include "verilated.h"

// Needed for waveform generation
#include <verilated_vcd_c.h>

#include <cassert>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    VStates* top = new VStates;
    Verilated::commandArgs(argc, argv);
    
    // For waveform generation
    unsigned long tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");

    struct inout {
        const uint32_t in;
        VStates::States__DOT__HighLow out;
    } inout[] = {
        /**
         * The enum is generated by Verilator from using
         * 
         * /* verilator public *\/
         * 
         * behind the typedef enum declaration. See the States.sv code.
         * 
         */
        { .in = 0x00000000, .out = VStates::States__DOT__HighLow::ZERO, },
        { .in = 0x00000001, .out = VStates::States__DOT__HighLow::LOW, },
        { .in = 0x00000010, .out = VStates::States__DOT__HighLow::LOW, },
        { .in = 0x7FFFFFFF, .out = VStates::States__DOT__HighLow::LOW, },
        { .in = 0x80000000, .out = VStates::States__DOT__HighLow::LOW, },
        { .in = 0x80000001, .out = VStates::States__DOT__HighLow::HIGH, },
        { .in = 0x80000002, .out = VStates::States__DOT__HighLow::HIGH, },
        { .in = 0x87236211, .out = VStates::States__DOT__HighLow::HIGH, },
        { .in = 0xFFFFFFFE, .out = VStates::States__DOT__HighLow::HIGH, },
        { .in = 0xFFFFFFFF, .out = VStates::States__DOT__HighLow::MAX, },
    };

    for (int i = 0; i < sizeof(inout) / sizeof(inout[0]); i++) {
        if (Verilated::gotFinish()) break;
        
        top->in = inout[i].in;
        
        top->eval();
        assert(top->state_out == inout[i].out);

        trace->dump(10*tickcount++);
    }

    trace->dump(10*tickcount++);
    
    trace->close();
    delete trace;

    delete top;
    return 0;
}