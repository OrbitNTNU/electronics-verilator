module Delay #(int G_NCYCLES = 3)(
    input clk,
    input[31:0] data_in,
    input data_in_valid,
    output[31:0] delayed_data,
    output delayed_data_valid
);

    reg [31:0] count = 0;
    reg [31:0] data;
    reg reg_delayed_data_valid = 0;

    always @(posedge(clk)) begin
        reg_delayed_data_valid <= 0;

        if (data_in_valid) begin
            data <= data_in;
            count <= G_NCYCLES;
        end else if (count == 0) begin
            reg_delayed_data_valid <= 1;
        end else begin
            count <= count - 1;
        end
    end

    assign delayed_data = data;
    assign delayed_data_valid = reg_delayed_data_valid;

endmodule
