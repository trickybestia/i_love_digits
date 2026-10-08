module pixel_mem #(
    parameter PIXEL_OUT_COUNT   = 2,
    parameter CURSOR_SIZE_WIDTH = 6,
    parameter PAINT_POS_WIDTH   = 9,
    parameter PAINT_POS_MAX     = 479
) (
    input clk,

    input  [PAINT_POS_WIDTH - 1:0]   paint_x,
    input  [PAINT_POS_WIDTH - 1:0]   paint_y,
    input                            paint_color,
    input  [CURSOR_SIZE_WIDTH - 1:0] paint_size,
    input                            paint_valid,
    output                           paint_ready,

    output [PIXEL_OUT_COUNT - 1:0] pixel_out,
    output [PIXEL_OUT_COUNT - 1:0] pixel_out_valid,
    input  [PIXEL_OUT_COUNT - 1:0] pixel_out_ready
);

localparam PIXELS_COUNT            = (PAINT_POS_MAX + 1) ** 2;
localparam PIXELS_COUNTER_WIDTH    = $clog2(PIXELS_COUNT);
localparam PIXEL_OUT_COUNTER_WIDTH = $clog2(PIXEL_OUT_COUNT);

enum {IDLE, PAINT, PIXEL_OUT} state;

logic mem [0:PIXELS_COUNT - 1];
logic mem_read_data;

logic [PIXEL_OUT_COUNTER_WIDTH - 1:0] selected_pixel_out;
logic [PIXELS_COUNTER_WIDTH - 1:0]    pixels_counter;

logic [PAINT_POS_WIDTH - 1:0]   paint_x_reg;
logic [PAINT_POS_WIDTH - 1:0]   paint_y_reg;
logic                           paint_color_reg;
logic [CURSOR_SIZE_WIDTH - 1:0] paint_size_reg;
logic                           paint_valid_reg;

logic [PAINT_POS_WIDTH - 1:0] paint_start_x, paint_end_x, paint_current_x;
logic [PAINT_POS_WIDTH - 1:0] paint_start_y, paint_end_y, paint_current_y;

assign paint_ready = !paint_valid_reg;

assign pixel_out       = {PIXEL_OUT_COUNT{mem_read_data}};
assign pixel_out_valid = (state == PIXEL_OUT) ? 1 << selected_pixel_out : 0;

initial begin
    state              <= IDLE;
    selected_pixel_out <= 0;
    pixels_counter     <= 0;
    paint_valid_reg    <= 0;
end

// state
always_ff @(posedge clk) begin
    case (state)
        IDLE: begin
            if (paint_valid_reg) begin
                state <= PAINT;
            end else if (|pixel_out_ready) begin
                state <= PIXEL_OUT;
            end
        end
        PAINT: begin
            if (paint_current_y == paint_end_y && paint_current_x == paint_end_x) begin
                state <= IDLE;
            end
        end
        PIXEL_OUT: begin
            if (pixels_counter == PIXELS_COUNT - 1 && pixel_out_valid[selected_pixel_out] && pixel_out_ready[selected_pixel_out]) begin
                state <= IDLE;
            end
        end
    endcase
end

// mem, mem_read_data
always_ff @(posedge clk) begin
    if (state == PAINT) begin
        mem[paint_current_y * (PAINT_POS_MAX + 1) + paint_current_x] <= paint_color_reg;
    end else begin
        mem_read_data <= mem[pixels_counter];
    end
end

// selected_pixel_out
always_ff @(posedge clk) begin
    if (state == IDLE && !paint_valid_reg && |pixel_out_ready) begin
        for (integer i = 1; i != PIXEL_OUT_COUNT + 1; i++) begin
            if (pixel_out_ready[(selected_pixel_out + i) % PIXEL_OUT_COUNT]) begin
                selected_pixel_out <= (selected_pixel_out + i) % PIXEL_OUT_COUNT;

                break;
            end
        end
    end
end

// pixels_counter
always_ff @(posedge clk) begin
    if (state == PIXEL_OUT && pixel_out_valid[selected_pixel_out] && pixel_out_ready[selected_pixel_out]) begin
        pixels_counter <= pixels_counter == PIXELS_COUNT - 1 ? 0 : pixels_counter + 1;
    end
end

// paint_x_reg
always_ff @(posedge clk) begin
    if (paint_valid && paint_ready) begin
        paint_x_reg <= paint_x;
    end
end

// paint_y_reg
always_ff @(posedge clk) begin
    if (paint_valid && paint_ready) begin
        paint_y_reg <= paint_y;
    end
end

// paint_color_reg
always_ff @(posedge clk) begin
    if (paint_valid && paint_ready) begin
        paint_color_reg <= paint_color;
    end
end

// paint_size_reg
always_ff @(posedge clk) begin
    if (paint_valid && paint_ready) begin
        paint_size_reg <= paint_size;
    end
end

// paint_valid_reg
always_ff @(posedge clk) begin
    if (state == PAINT && paint_current_y == paint_end_y && paint_current_x == paint_end_x) begin
        paint_valid_reg <= 0;
    end else if (paint_valid && paint_ready) begin
        paint_valid_reg <= paint_valid;
    end
end

// paint_start_x, paint_end_x, paint_current_x
always_ff @(posedge clk) begin
    case (state)
        IDLE: begin
            if (paint_valid_reg) begin
                if (paint_size > paint_x) begin
                    paint_start_x   <= 0;
                    paint_current_x <= 0;
                end else begin
                    paint_start_x   <= paint_x - paint_size;
                    paint_current_x <= paint_x - paint_size;
                end

                if (paint_x + paint_size > PAINT_POS_MAX) begin
                    paint_end_x <= PAINT_POS_MAX;
                end else begin
                    paint_end_x <= paint_x + paint_size;
                end
            end
        end
        PAINT: begin
            paint_current_x <= paint_current_x == paint_end_x ? paint_start_x : paint_current_x + 1;
        end
    endcase
end

// paint_start_y, paint_end_y, paint_current_y
always_ff @(posedge clk) begin
    case (state)
        IDLE: begin
            if (paint_valid_reg) begin
                if (paint_size > paint_y) begin
                    paint_start_y   <= 0;
                    paint_current_y <= 0;
                end else begin
                    paint_start_y   <= paint_y - paint_size;
                    paint_current_y <= paint_y - paint_size;
                end

                if (paint_y + paint_size > PAINT_POS_MAX) begin
                    paint_end_y <= PAINT_POS_MAX;
                end else begin
                    paint_end_y <= paint_y + paint_size;
                end
            end
        end
        PAINT: begin
            if (paint_current_x == paint_end_x) begin
                paint_current_y <= paint_current_y + 1;
            end
        end
    endcase
end

endmodule
