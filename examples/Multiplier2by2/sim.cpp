#include "VMultiplier2by2.h"
#include "verilated.h"

#include <cassert>
#include <iostream>

#include <verilated_vcd_c.h>

//Driver code 
int main(int argc, char** argv) {
    VMultiplier2by2* top = new VMultiplier2by2;
    Verilated::commandArgs(argc, argv);

    // For waveform generation
    unsigned long tickcount = 0;
    Verilated::traceEverOn(true);

    // Initialization of trace
    VerilatedVcdC *trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");

    uint8_t stimuli_a[16];
    uint8_t stimuli_b[16];
    int counter_to_16 = 0;
    for (uint8_t i = 0; i <= 0b11; ++i) {       
        for (uint8_t j = 0; j <= 0b11; ++j) {
            stimuli_a[counter_to_16] = i;
            stimuli_b[counter_to_16] = j;
            counter_to_16++;
        }
    }

    int ret = 0;
    for (int i = 0; i < sizeof(stimuli_a) / sizeof(stimuli_a[0]); i++) {
        if (Verilated::gotFinish()) break;

        top->A = stimuli_a[i];
        top->B = stimuli_b[i];
        
        top->eval();
        
        if(top->Prod != (stimuli_a[i] * stimuli_b[i])) {
            printf("%i\n",i);
            printf("prod: 0x%x\n", top->Prod);
            printf("Should be: 0x%x\n", (stimuli_a[i] * stimuli_b[i]));            
            fflush(stdout);
            ret += 1;
            break;
        }
    
        // For trace; dump all variables
        trace->dump(10*tickcount++);
    }

    // Dump a final time to avoid early cutoff
    trace->dump(10*tickcount++);

    trace->close();
    delete trace;

    delete top;
    return ret;
} 
