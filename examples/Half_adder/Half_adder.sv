/* half_adder
*/
module Half_adder(A,B,sum,carry);
    input A, B;
    output sum, carry;
    xor x1(sum,A,B);
    and a1(carry,A,B);
endmodule