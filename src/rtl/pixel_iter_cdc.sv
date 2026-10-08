module pixel_iter_cdc #(
    parameter PIXELS_COUNT = 480 * 480
) (
    input pixel_in_clk,
    input pixel_out_clk,

    input  pixel_in,
    input  pixel_in_valid,
    output pixel_in_ready,

    output pixel_out,
    output pixel_out_valid,
    input  pixel_out_ready
);

localparam PIXELS_COUNTER_WIDTH = $clog2(PIXELS_COUNT);

// clocked by pixel_in_clk:
logic iter_start_req;
logic [PIXELS_COUNTER_WIDTH - 1:0] pixel_in_counter;
logic fifo_full;

assign pixel_in_ready = (iter_start_req || pixel_in_counter != 0) && !fifo_full;

// clocked by pixel_out_clk:
logic [PIXELS_COUNTER_WIDTH - 1:0] pixel_out_counter;

xpm_cdc_array_single #(
   .DEST_SYNC_FF   (4), // DECIMAL; range: 2-10
   .INIT_SYNC_FF   (1), // DECIMAL; 0=disable simulation init values, 1=enable simulation init values
   .SIM_ASSERT_CHK (1), // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
   .SRC_INPUT_REG  (1), // DECIMAL; 0=do not register input, 1=register input
   .WIDTH          (1)  // DECIMAL; range: 1-1024
) xpm_cdc_array_single_inst (
   .src_clk  (pixel_out_clk),                             // 1-bit input: Optional; required when SRC_INPUT_REG = 1
   .src_in   (pixel_out_counter == 0 && pixel_out_ready), // WIDTH-bit input: Input single-bit array that synchronizes to the destination clock domain.
   .dest_clk (pixel_in_clk),                              // 1-bit input: Clock signal for the destination clock domain.
   .dest_out (iter_start_req)                             // WIDTH-bit output: This registered output synchronizes src_in to the destination clock domain.
);

xpm_fifo_async #(
    .CASCADE_HEIGHT      (0),         // DECIMAL
    .CDC_SYNC_STAGES     (2),         // DECIMAL
    .DOUT_RESET_VALUE    ("0"),       // String
    .ECC_MODE            ("no_ecc"),  // String
    .EN_SIM_ASSERT_ERR   ("warning"), // String
    .FIFO_MEMORY_TYPE    ("auto"),    // String
    .FIFO_READ_LATENCY   (0),         // DECIMAL
    .FIFO_WRITE_DEPTH    (16),        // DECIMAL
    .FULL_RESET_VALUE    (0),         // DECIMAL
    .PROG_EMPTY_THRESH   (10),        // DECIMAL
    .PROG_FULL_THRESH    (10),        // DECIMAL
    .RD_DATA_COUNT_WIDTH (1),         // DECIMAL
    .READ_DATA_WIDTH     (1),         // DECIMAL
    .READ_MODE           ("fwft"),    // String
    .RELATED_CLOCKS      (0),         // DECIMAL
    .SIM_ASSERT_CHK      (1),         // DECIMAL; 0=disable simulation messages, 1=enable simulation messages
    .USE_ADV_FEATURES    ("1000"),    // String
    .WAKEUP_TIME         (0),         // DECIMAL
    .WRITE_DATA_WIDTH    (1),         // DECIMAL
    .WR_DATA_COUNT_WIDTH (1)          // DECIMAL
) fifo (
    .sleep  (0),            // 1-bit input: Dynamic power saving: If sleep is High, the memory/fifo block is in power saving mode.
    .wr_clk (pixel_in_clk), // 1-bit input: Write clock: Used for write operation. wr_clk must be a free running clock.
    .rst    (0),            // 1-bit input: Reset: Must be synchronous to wr_clk. The clocks can be unstable at the time of applying reset,
                            // but release reset only after the clocks is/are stable.

    .wr_rst_busy (),                                 // 1-bit output: Write Reset Busy: Active-High indicator that the FIFO write domain remains in a reset state.
    .wr_en       (pixel_in_valid && pixel_in_ready), // 1-bit input: Write Enable: If the FIFO is not full, asserting this signal writes data (on din) to the FIFO.
                                                     // Hold this signal active-Low when rst or wr_rst_busy is active-High.

    .din    (pixel_in), // WRITE_DATA_WIDTH-bit input: Write Data: The input data bus used when writing the FIFO.
    .wr_ack (),         // 1-bit output: Write Acknowledge: This signal indicates that a write request (wr_en) during the prior clock
                        // cycle succeeded.

    .full (fifo_full), // 1-bit output: Full Flag: When asserted, this signal indicates that the FIFO is full. The FIFO ignores write
                       // requests when the FIFO is full, initiating a write when the FIFO is full is not destructive to the contents
                       // of the FIFO.

    .prog_full (), // 1-bit output: Programmable Full: This signal asserts when the number of words in the FIFO is greater than or
                   // equal to the programmable full threshold value. It de-asserts when the number of words in the FIFO is less
                   // than the programmable full threshold value.

    .almost_full (), // 1-bit output: Almost Full: When asserted, this signal indicates that the FIFO can perform only one more write
                     // before it is full.

    .overflow (), // 1-bit output: Overflow: This signal indicates that the FIFO rejected a write request (wren) during the prior
                  // clock cycle because the FIFO was full. Overflowing the FIFO is not destructive to the contents of the FIFO.

    .wr_data_count (), // WR_DATA_COUNT_WIDTH-bit output: Write Data Count: This bus indicates the number of words written into the
                       // FIFO.

    .rd_clk      (pixel_out_clk),                      // 1-bit input: Read clock: Used for read operation. rd_clk must be a free running clock.
    .rd_rst_busy (),                                   // 1-bit output: Read Reset Busy: Active-High indicator that the FIFO read domain remains in a reset state.
    .rd_en       (pixel_out_valid && pixel_out_ready), // 1-bit input: Read Enable: If the FIFO is not empty, asserting this signal reads data (on dout) from the FIFO.
                                                       // Hold this signal active-Low when rd_rst_busy is active-High.

    .dout       (pixel_out),       // READ_DATA_WIDTH-bit output: Read Data: This drives the output data bus when reading the FIFO.
    .data_valid (pixel_out_valid), // 1-bit output: Read Data Valid: When asserted, this signal indicates that valid data appears on the output bus
                                   // (dout).

    .empty (), // 1-bit output: Empty Flag: When asserted, this signal indicates that the FIFO is empty. The FIFO ignores read
               // requests when the FIFO is empty, initiating a read while empty is not destructive to the FIFO.

    .prog_empty (), // 1-bit output: Programmable Empty: This signal asserts when the number of words in the FIFO is less than or
                    // equal to the programmable empty threshold value. It deasserts when the number of words in the FIFO exceeds
                    // the programmable empty threshold value.

    .almost_empty (), // 1-bit output: Almost Empty: When asserted, this signal indicates that the FIFO can provide only one more read
                      // before it goes to empty.

    .underflow (), // 1-bit output: Underflow: Indicates that the read request (rd_en) during the previous clock cycle was rejected
                   // because the FIFO is empty. Underflowing the FIFO is not destructive to the FIFO.

    .rd_data_count (), // RD_DATA_COUNT_WIDTH-bit output: Read Data Count: This bus indicates the number of words read from the FIFO.
    .injectsbiterr (), // 1-bit input: Single Bit Error Injection: Injects a single bit error if using the ECC feature on block RAMs.
    .injectdbiterr (), // 1-bit input: Double Bit Error Injection: Injects a double bit error if using the ECC feature on block RAMs.
    .sbiterr       (), // 1-bit output: Single Bit Error: Indicates that the ECC decoder detected and fixed a single-bit error.
    .dbiterr       ()  // 1-bit output: Double Bit Error: Indicates that the ECC decoder detected a double-bit error and data in the
                       // FIFO core becomes corrupted.
);

initial begin
    pixel_in_counter  <= 0;
    pixel_out_counter <= 0;
end

// pixel_in_counter
always_ff @(posedge pixel_in_clk) begin
    if (pixel_in_valid && pixel_in_ready) begin
        pixel_in_counter <= pixel_in_counter == PIXELS_COUNT - 1 ? 0 : pixel_in_counter + 1;
    end
end

// pixel_out_counter
always_ff @(posedge pixel_out_clk) begin
    if (pixel_out_valid && pixel_out_ready) begin
        pixel_out_counter <= pixel_out_counter == PIXELS_COUNT - 1 ? 0 : pixel_out_counter + 1;
    end
end

endmodule
