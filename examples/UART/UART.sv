module UART (
    input clk,
    input reset,
    input uart_enable,
    input to_uart_rx,

    output to_uart_tx,
);

    // TX

    RX #(
        .G_CLK_FREQ(),
        .G_BAUDRATE()
    ) UART_rx (
        .clk(clk),
        .reset(reset),
        .enable(enable),
        .rx_line(to_uart_rx),

        .rdy()
    )

endmodule
