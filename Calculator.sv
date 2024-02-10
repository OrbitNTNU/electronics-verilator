//add=and substract=and multiply=develop divide=develop
//only 3*4 bit multiplier because 7*15<128 but 15*15>128
//input from user and conversion to binary in c++
//conversion back to integer in c++ too, include negative!(signed bit)

module And (
    input a,
    input b,
    output c
);

    assign c = a & b;

endmodule

//two bytes, take last 4 bits of first and last 3 bits of second
//also check if negative
//first 2 by 2 multiplier:
//example: 
//00000010*00000010
//->000000 0&1 0&0 -> 00000000
//->000000 1&1 1&0 -> 00000010 leftshift-> 00000100
