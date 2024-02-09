module DoubleSub (
    input[31:0] a1,
    input[31:0] a2,
    input[31:0] b1,
    input[31:0] b2,
    output[31:0] res1,
    output[31:0] res2
);

    Sub sub1 (
        .a(a1),
        .b(b1),
        .res(res1)
    );

    Sub sub2 (
        .a(a2),
        .b(b2),
        .res(res2)
    );

endmodule
