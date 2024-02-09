#include "VDoubleSub.h"
#include "verilated.h"

#include <cassert>
#include <iostream>

int get_stimuli(std::string filename, int *data);

int main(int argc, char** argv) {
    VDoubleSub* top = new VDoubleSub;
    Verilated::commandArgs(argc, argv);

    int *operands = new int[200];
    int noperands = get_stimuli(std::string{"operands.data"}, operands);
    int nlines = noperands / 4;

    for (int i = 0; i < nlines; i++) {
        top->a1 = operands[i * 4 + 0];
        top->a2 = operands[i * 4 + 1];
        top->b1 = operands[i * 4 + 2];
        top->b2 = operands[i * 4 + 3];

        top->eval();

        assert(top->res1 == (top->a1 - top->b1));
        assert(top->res2 == (top->a2 - top->b2));
    }


    delete top;
    return 0;
}
