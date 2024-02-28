module FA(
    input a,b,cin,
    output sum,cout
);

wire w1,w2,w3,w4;       //Internal connections

xor(w1,a,b);
xor(sum,w1,cin);        //Sum output

and(w2,a,b);
and(w3,b,cin);
and(w4,cin,a);

or(cout,w2,w3,w4);     //carry output

endmodule