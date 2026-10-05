module cursor_controller #(
    parameter DX_WIDTH          = 10,
    parameter DY_WIDTH          = 10,
    parameter MOVE_SCALE_MUL    = 1,
    parameter MOVE_SCALE_DIV    = 1,
    parameter CURSOR_X_MAX      = 639,
    parameter CURSOR_Y_MAX      = 479,
    parameter CURSOR_X_WIDTH    = $clog2(CURSOR_X_MAX + 1),
    parameter CURSOR_Y_WIDTH    = $clog2(CURSOR_Y_MAX + 1),
    parameter CURSOR_SIZE_MAX   = 63,
    parameter CURSOR_SIZE_WIDTH = 6,
    parameter PAINT_POS_WIDTH   = 9
) (
    input clk,

    input signed [DX_WIDTH - 1:0]  dx,
    input signed [DY_WIDTH - 1:0]  dy,
    input                          scroll_up,
    input                          scroll_down,
    input                          lmb_pressed,
    input                          rmb_pressed,
    input                          input_valid,
    output                         input_ready,

    output [PAINT_POS_WIDTH - 1:0]   paint_x,
    output [PAINT_POS_WIDTH - 1:0]   paint_y,
    output                           paint_color,
    output [CURSOR_SIZE_WIDTH - 1:0] paint_size,
    output                           paint_valid,
    input                            paint_ready,

    output logic [CURSOR_X_WIDTH - 1:0]    cursor_x,
    output logic [CURSOR_Y_WIDTH - 1:0]    cursor_y,
    output logic [CURSOR_SIZE_WIDTH - 1:0] cursor_size
);

wire signed [CURSOR_SIZE_WIDTH + 2 - 1:0] cursor_size_next_extended = cursor_size + scroll_up - scroll_down;
wire signed [CURSOR_X_WIDTH + 2 - 1:0]    cursor_x_next_extended    = cursor_x + dx * MOVE_SCALE_MUL / MOVE_SCALE_DIV;
wire signed [CURSOR_Y_WIDTH + 2 - 1:0]    cursor_y_next_extended    = cursor_y + dy * MOVE_SCALE_MUL / MOVE_SCALE_DIV;

assign input_ready = paint_ready;

assign paint_x     = cursor_x;
assign paint_y     = cursor_y;
assign paint_color = lmb_pressed;
assign paint_size  = cursor_size;
assign paint_valid = input_valid && (lmb_pressed || rmb_pressed);

// cursor_x
always_ff @(posedge clk) begin
    if (input_valid) begin
        if (cursor_x_next_extended < 0)                 cursor_x <= 0;
        else if (cursor_x_next_extended > CURSOR_X_MAX) cursor_x <= CURSOR_X_MAX;
        else                                            cursor_x <= cursor_x_next_extended;
    end
end

// cursor_y
always_ff @(posedge clk) begin
    if (input_valid) begin
        if (cursor_y_next_extended < 0)                 cursor_y <= 0;
        else if (cursor_y_next_extended > CURSOR_Y_MAX) cursor_y <= CURSOR_Y_MAX;
        else                                            cursor_y <= cursor_y_next_extended;
    end
end

// cursor_size
always_ff @(posedge clk) begin
    if (input_valid) begin
        if (cursor_size_next_extended < 0)                    cursor_size <= 0;
        else if (cursor_size_next_extended > CURSOR_SIZE_MAX) cursor_size <= CURSOR_SIZE_MAX;
        else                                                  cursor_size <= cursor_size_next_extended;
    end
end

endmodule
