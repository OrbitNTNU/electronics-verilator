module MUX2to1(
    input a,b,X,
    output Q
);

    wire w1,w2,Xnt;

    nand(Xnt,X,X);
    nand(w1,a,X);
    nand(w2,b,Xnt);

    nand(Q,w1,w2); 

endmodule