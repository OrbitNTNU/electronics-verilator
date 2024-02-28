`include "../PU3/PU3.sv"

module Divider4by2(
    input [3:0] A,
    input [1:0] B,
    output [3:0] Q,
    output [4:0] R //trash
);
    wire w1,w2,w3,w4,w5;          //needed

    PU2 pu5(A[3],0 ,   B[0],B[1],  1,Q[3],w1  ,R[3]     );
    PU2 pu6(A[2],w1,   B[0],B[1],  1,Q[2],w2  ,w3       );
    PU3 pu7(A[1],w2,w3,B[0],B[1],0,1,Q[1],w4  ,w5  ,R[4]);
    PU3 pu8(A[0],w4,w5,B[0],B[1],0,1,Q[0],R[0],R[1],R[2]);
   
endmodule