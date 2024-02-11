#include "VHalf_adder.h"
#include "verilated.h"

#include <cassert>
#include <iostream>

int get_stimuli(std::string filename, int *data);

int main(int argc, char** argv) {
    VHalf_adder* top = new VHalf_adder;
    Verilated::commandArgs(argc, argv);

    int *operands = new int[200];
    int noperands = get_stimuli(std::string{"operands.data"}, operands);
    int nlines = noperands / 2;  // Change to 2 since you have two inputs (a and b) per line

    for (int i = 0; i < nlines; i++) {
        top->a = operands[i * 2 + 0];  
        top->b = operands[i * 2 + 1];  

        top->eval();

        assert(top->sum == (top->a ^ top->b));     // Check the output 'sum'
        assert(top->carry == (top->a & top->b));   // Check the output 'carry'
    }

    delete top;
    return 0;
}
