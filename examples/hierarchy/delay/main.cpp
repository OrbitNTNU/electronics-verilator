#include "VDelay.h"
#include "verilated.h"

#include <cassert>
#include <iostream>

int main(int argc, char** argv) {
    VerilatedContext* contextp = new VerilatedContext;
    contextp->commandArgs(argc, argv);
    VDelay* top = new VDelay{contextp};

    int data_in = 0x66234223;
    
    top->clk = 0;
    top->eval();

    top->clk = 1;
    top->data_in = data_in;
    top->data_in_valid = true;
    top->eval();
    assert(top->delayed_data_valid == false);

    top->clk = 0;
    top->data_in_valid = false;
    top->eval();

    for (int i = 0; i < 3; i++) {
        top->clk = 1;
        top->eval();
        assert(top->delayed_data_valid == false);
        top->clk = 0;
        top->eval();
        assert(top->delayed_data_valid == false);
    }

    top->clk = 1;
    top->eval();

    assert(top->delayed_data_valid == true);
    assert(top->delayed_data == data_in);

    delete top;
    delete contextp;
    return 0;
}
