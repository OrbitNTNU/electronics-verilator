`include "../PU/PU.sv"

module PU2(
    input D0,D1,d0,d1,cin,
    output cout,R0,R1
);

wire c1;       //Internal connections

PU pu1(D1,d1,c1,cout,cout,R1);
PU pu2(D0,d0,cin,R1,c1,R0);

endmodule