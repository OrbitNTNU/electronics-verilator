module UART_rx #(
    int G_CLK_FREQ = 100000000,
    int G_BAUDRATE = 115200
) (
    input clk,
    input reset,
    input enable,
    input rx_line,

    output rdy,
    output rx_byte_valid,
    output rx_byte_error,
    output [7:0] rx_byte
);

    parameter C_COUNTER_LIMIT = G_CLK_FREQ / G_BAUDRATE;

    typedef enum logic[1:0] { 
        RX_IDLE,
        RX_START_BIT,
        RX_DATA_BITS,
        RX_STOP_BIT
    } UART_rx_state /* verilator public */;

    UART_rx_state state = RX_IDLE;

    reg r_first_byte;
    reg r_rdy;
    reg r_rx_byte_valid;
    reg r_rx_byte_error;
    reg [7:0] r_byte_out;
    reg [31:0] clk_counter;
    reg [2:0] bit_counter;

    always @(posedge clk) begin
        if (reset == 1) 
        begin
            r_first_byte <= 1;
            r_rdy <= 0;
            r_rx_byte_valid <= 0;
            r_rx_byte_error <= 0;
            r_byte_out <= 0;
            clk_counter <= 0;
            bit_counter <= 0;
            state <= RX_IDLE;
        end
        else if (enable == 1)
        begin
            r_rdy <= 0;
            clk_counter <= clk_counter + 1;
            r_rx_byte_valid <= 0;

            case (state)
                RX_IDLE:
                begin
                    r_rx_byte_valid <= !r_first_byte && !r_rx_byte_error;
                    clk_counter <= 0;
                    r_rdy <= 1;
                    if (rx_line == 0) begin
                        state <= RX_START_BIT;
                        r_rx_byte_error <= 0;
                    end
                end
                
                RX_START_BIT: 
                begin
                    if (clk_counter == (C_COUNTER_LIMIT / 2))
                    begin
                        clk_counter <= 0;
                        state <= RX_DATA_BITS;
                    end
                end
                
                RX_DATA_BITS: 
                begin
                    if (clk_counter == C_COUNTER_LIMIT)
                    begin
                        clk_counter <= 0;
                        r_byte_out[bit_counter] <= rx_line;
                        
                        if (bit_counter == 7) begin
                            state <= RX_STOP_BIT;
                            bit_counter <= 0;
                        end else begin
                            bit_counter <= bit_counter + 1;
                        end
                    end
                end
                
                RX_STOP_BIT: 
                begin
                    if (clk_counter == C_COUNTER_LIMIT)
                    begin
                        // If rx_line is high, there is no error
                        if (rx_line) begin
                            r_rx_byte_valid <= 1;
                            r_first_byte <= 0;
                            state <= RX_IDLE;
                        end else 
                            r_rx_byte_error <= !rx_line;
                        clk_counter <= 0;
                    end else if (r_rx_byte_error) begin
                        if (rx_line) begin
                            r_rx_byte_valid <= 0;
                            r_first_byte <= 1;
                            state <= RX_IDLE;
                        end
                    end
                end
            endcase
        end
    end

    assign rdy = r_rdy;
    assign rx_byte_valid = r_rx_byte_valid;
    assign rx_byte = r_byte_out;
    assign rx_byte_error = r_rx_byte_error;

endmodule
