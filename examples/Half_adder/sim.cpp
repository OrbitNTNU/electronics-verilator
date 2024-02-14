#include "VHalf_adder.h"
#include "verilated.h"

#include <cassert>
#include <iostream>

#include <verilated_vcd_c.h>

//Driver code 
int main(int argc, char** argv) {
    VHalf_adder* top = new VHalf_adder;
    Verilated::commandArgs(argc, argv);

    // For waveform generation
    unsigned long tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");

    bool stimuli_a[] = { false, false, true, true };
    bool stimuli_b[] = { false, true, false, true }; 

    for (int i = 0; i < sizeof(stimuli_a) / sizeof(stimuli_a[0]); i++) {
        if (Verilated::gotFinish()) break;

        top->A = stimuli_a[i];
        top->B = stimuli_b[i];
        
        top->eval();

        assert(top->carry == (stimuli_a[i] && stimuli_b[i]));
        assert(top->sum == (stimuli_a[i] ^ stimuli_b[i]));
    
        // For trace; dump all variables
        trace->dump(10*tickcount++);
    }

    // Dump a final time to avoid early cutoff
    trace->dump(10*tickcount++);

    trace->close();
    delete trace;

    delete top;
    return 0;
} 
