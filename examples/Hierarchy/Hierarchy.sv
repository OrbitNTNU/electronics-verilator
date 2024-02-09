`include "../Doublesub/Sub.sv"
`include "Delay/Delay.sv"

module Hierarchy (
    input clk,
    input[31:0] data_in,
    input[31:0] subtract,
    input data_in_valid,
    output[31:0] data_out,
    output data_out_valid
);

    wire w_data_out_valid;
    wire[31:0] w_delayed_data;
    wire[31:0] w_data_out;

    assign w_data_out = (w_data_out_valid == 1) ? w_delayed_data : 0;  


    Delay #(.G_NCYCLES(5)) delay (
        .clk(clk),
        .data_in(data_in),
        .data_in_valid(data_in_valid),
        .delayed_data(w_delayed_data),
        .delayed_data_valid(w_data_out_valid)
    );

    Sub sub (
        .a(w_data_out),
        .b(subtract),
        .res(data_out)
    );

    assign data_out_valid = w_data_out_valid;

endmodule
