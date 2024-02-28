`include "../PU2/PU2.sv"

module PU3(
    input D0,D1,D2,d0,d1,d2,cin,
    output cout,R0,R1,R2
);

wire c1;       //Internal connections

PU2 pu4(D1,D2,d1,d2,c1,cout,R1,R2);
PU pu3(D0,d0,cin,R1,c1,R0);

endmodule
