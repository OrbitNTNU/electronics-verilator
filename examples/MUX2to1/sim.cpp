#include "VMUX2to1.h"
#include "verilated.h"

// Needed for waveform generation
#include <verilated_vcd_c.h>

#include <cassert>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    VMUX2to1* top = new VMUX2to1;
    Verilated::commandArgs(argc, argv);

    // For waveform generation
    unsigned long tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");

    // All inputs
    bool stimuli[] = { false, true };

    int iterations = 8;
    int j=0;
    int k=0;  

    for (int i = 0; i < iterations; i++) {
        if (Verilated::gotFinish()) break;

        //clocktype
        int c = i%2;
        int c2 = j%2;
        int c3 = k%2;

        top->a = stimuli[c];
        top->b = stimuli[c2];

        top->X = stimuli[c3];
        
        top->eval();

        bool Xnt_ = !(stimuli[c3] && stimuli[c3]);
        bool w1_ = !(stimuli[c3] && stimuli[c]);
        bool w2_ = !(stimuli[c2] && Xnt_);

        assert(top->Q == !(w1_ && w2_) );

        //clocktype
        if (i%2 == 1) { //after every second i
            j++;
        }
               
        if ((i+1)%4 == 0) { //after every fourth i
            k++;
        }
        
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
