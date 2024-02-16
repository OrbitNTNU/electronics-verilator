`include "../Half_adder/Half_adder.sv"
`include "../And/And.sv"

module Multiplier2by2(A,B,Prod);

    input [1:0] A,B;
    output [3:0] Prod;
    wire w1,w2,w3,C;

    And n1(A[0],B[0],Prod[0]);

    And n2(A[0],B[1],w1);
    And n3(A[1],B[0],w2);
    Half_adder n4(w1,w2,Prod[1],C);

    And n5(A[1],B[1],w3);
    Half_adder n6(w3,C,Prod[2],Prod[3]);

endmodule