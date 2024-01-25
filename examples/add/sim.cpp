#include "VAdd.h"
#include "verilated.h"

#include <cassert>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    VAdd* top = new VAdd{contextp};
    
    int stimuli_a[] = { 2, -1, 1, -6 };
    int stimuli_b[] = { 4,  5, 2,  7 }; 

    for (int i = 0; i < sizeof(stimuli_a) / sizeof(stimuli_a[0]); i++) {
        if (contextp->gotFinish()) break;
        
        top->a = stimuli_a[i];
        top->b = stimuli_b[i];
        
        top->eval();

        assert(top->c == (stimuli_a[i] + stimuli_b[i]));
    }
    
    delete top;
    delete contextp;
    return 0;
}
