module neural_network_out_cdc (
    input clk_nn,
    input clk_vga,

    input [6:0] score_in,
    input       score_in_valid,
    output      score_in_ready,

    output [6:0][0:9] score_out
);

logic [6:0][0:9] score_out_nn;
logic [3:0]      score_out_nn_counter;

assign score_in_ready = score_out_nn_counter != 10;

xpm_cdc_handshake #(
    .DEST_EXT_HSK   (0), // DECIMAL; 0=internal handshake, 1=external handshake
    .DEST_SYNC_FF   (4), // DECIMAL; range: 2-10
    .INIT_SYNC_FF   (1), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
    .SIM_ASSERT_CHK (1), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
    .SRC_SYNC_FF    (4), // DECIMAL; range: 2-10
    .WIDTH          ($bits(score_out)) // DECIMAL; range: 1-1024
) xpm_cdc_handshake_inst (
    .src_clk  (clk_nn),
    .src_in   (score_out_nn),
    .src_send (score_out_nn_counter == 10),

    .dest_clk (clk_vga),
    .dest_out (score_out)
);

initial begin
    score_out_nn         <= '0;
    score_out_nn_counter <= '0;
end

// score_out_nn
always_ff @(posedge clk_nn) begin
    if (score_in_valid && score_in_ready) begin
        score_out_nn[score_out_nn_counter] <= score_in;
    end
end

// score_out_nn_counter
always_ff @(posedge clk_nn) begin
    if (score_out_nn_counter == 10) begin
        score_out_nn_counter <= 0;
    end else if (score_in_valid && score_in_ready) begin
        score_out_nn_counter <= score_out_nn_counter + 1;
    end
end

endmodule
