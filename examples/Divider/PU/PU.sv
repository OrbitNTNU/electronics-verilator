`include "../../FA/FA.sv"
`include "../../MUX2to1/MUX2to1.sv"

module PU(
    input a,b,cin,min,
    output cout, mout
);
    wire w1;

    FA fa(a,b,cin,w1,cout);
    MUX2to1 mux(a,w1,min,mout);

endmodule