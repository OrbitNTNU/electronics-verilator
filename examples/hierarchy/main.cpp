#include "VHierarchy.h"
#include "verilated.h"

#include <cassert>
#include <iostream>

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    VHierarchy* top = new VHierarchy{contextp};

    int data_in = 0x66234223;
    int subtract = 22;
    
    top->clk = 0;
    top->eval();

    top->clk = 1;
    top->data_in = data_in;
    top->data_in_valid = true;
    top->subtract = subtract;
    top->eval();
    assert(top->data_out_valid == false);

    top->clk = 0;
    top->data_in_valid = false;
    top->eval();

    for (int i = 0; i < 5; i++) {
        top->clk = 1;
        top->eval();
        assert(top->data_out == -subtract);
        assert(top->data_out_valid == false);
        top->clk = 0;
        top->eval();
        assert(top->data_out == -subtract);
        assert(top->data_out_valid == false);
    }

    top->clk = 1;
    top->eval();

    assert(top->data_out_valid == true);
    assert(top->data_out == (data_in - subtract));

    delete top;
    delete contextp;
    return 0;
}
