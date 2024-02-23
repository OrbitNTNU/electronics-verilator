module States (
    input[31:0] in,
    output[1:0] state_out
);
    typedef enum logic [1:0] { ZERO, LOW, HIGH, MAX } HighLow /* verilator public */;
    typedef enum logic [2:0] { INIT, DATA_RX, DATA_TX, STOPBIT, RESET } UART /* verilator public */;
    HighLow state = ZERO;

    always @(in) begin
        if (in == 0)
            state = ZERO;
        else if (in <= 'h80000000)
            state = LOW;
        else if (in == 'hFFFFFFFF)
            state = MAX;
        else
            state = HIGH;    
    end

    assign state_out = state;

endmodule
