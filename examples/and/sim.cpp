#include "VAnd.h"
#include "verilated.h"

// Needed for waveform generation
#include <verilated_vcd_c.h>

#include <cassert>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    VAnd* top = new VAnd{contextp};

    return 0;

    // For waveform generation
    unsigned long tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");

    // All combinations
    bool stimuli_a[] = { false, false, true, true };
    bool stimuli_b[] = { false, true, false, true }; 

    for (int i = 0; i < sizeof(stimuli_a) / sizeof(stimuli_a[0]); i++) {
        if (contextp->gotFinish()) break;

        top->a = stimuli_a[i];
        top->b = stimuli_b[i];
        
        top->eval();

        assert(top->c == (stimuli_a[i] && stimuli_b[i]));
        
        // For trace; dump all variables
        trace->dump(10*tickcount++);
    }

    // Dump a final time to avoid early cutoff
    trace->dump(10*tickcount++);

    trace->close();
    delete trace;

    delete top;
    delete contextp;
    return 0;
}
