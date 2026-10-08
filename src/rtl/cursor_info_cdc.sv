module cursor_info_cdc #(
    parameter CURSOR_X_WIDTH    = 10,
    parameter CURSOR_Y_WIDTH    = 9,
    parameter CURSOR_SIZE_WIDTH = 6
) (
    input in_clk,
    input out_clk,

    input [CURSOR_X_WIDTH - 1:0]    cursor_x_in,
    input [CURSOR_Y_WIDTH - 1:0]    cursor_y_in,
    input [CURSOR_SIZE_WIDTH - 1:0] cursor_size_in,

    output [CURSOR_X_WIDTH - 1:0]    cursor_x_out,
    output [CURSOR_Y_WIDTH - 1:0]    cursor_y_out,
    output [CURSOR_SIZE_WIDTH - 1:0] cursor_size_out
);

xpm_cdc_handshake #(
    .DEST_EXT_HSK   (0), // DECIMAL; 0=internal handshake, 1=external handshake
    .DEST_SYNC_FF   (4), // DECIMAL; range: 2-10
    .INIT_SYNC_FF   (1), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
    .SIM_ASSERT_CHK (1), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
    .SRC_SYNC_FF    (4), // DECIMAL; range: 2-10
    .WIDTH          (CURSOR_X_WIDTH + CURSOR_Y_WIDTH + CURSOR_SIZE_WIDTH) // DECIMAL; range: 1-1024
) xpm_cdc_handshake_inst (
    .src_clk  (in_clk),
    .src_in   ({cursor_x_in, cursor_y_in, cursor_size_in}),
    .src_send (1),

    .dest_clk (out_clk),
    .dest_out ({cursor_x_out, cursor_y_out, cursor_size_out})
);

endmodule
