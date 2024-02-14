`include "../Half_adder/Half_adder.sv"

module Multiplier2by2(Prod,A,B);

    input [1:0] A,B;
    output [3:0] P;
    wire w1,w2,w3,C;

    and n1(P[0],A[0],B[0]);

    and n2(w1,A[0],B[1]);
    and n3(w2,A[1],B[0]);
    Half_adder n4(P[1],C,w1,w2);

    and n5(w3,A[1],B[1]);
    Half_adder n6(P[2],P[3],w3,C);

endmodule