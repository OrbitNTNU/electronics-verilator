#include "VFA.h"
#include "verilated.h"

// Needed for waveform generation
#include <verilated_vcd_c.h>

#include <cassert>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    VFA* top = new VFA;
    Verilated::commandArgs(argc, argv);

    // For waveform generation
    unsigned long tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");

    // All combinations
    bool stimuli_a[] = { false, true };
    bool stimuli_b[] = { false, true }; 
    bool stimuli_c[] = { false, true }; 

    int iterations = 8;
    int j=0;
    int k=0;  

    for (int i = 0; i < iterations; i++) {
        if (Verilated::gotFinish()) break;

        

        int c = i%2;
        int c2 = j%2;
        int c3 = k%2;

        top->a = stimuli_a[c3];
        top->b = stimuli_b[c2];

        top->cin = stimuli_c[c];
        
        top->eval();

        assert(top->sum == ((stimuli_a[c3]^stimuli_b[c2])^ stimuli_c[c]));
        assert(top->cout == (stimuli_c[c] && stimuli_a[c3]) || 
                            (stimuli_b[c2] && stimuli_c[c]) || 
                            (stimuli_b[c2] && stimuli_a[c3]));

        if (i%2 == 1) {
            j++;
        }
               
        if ((i+1)%4 == 0) {
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
