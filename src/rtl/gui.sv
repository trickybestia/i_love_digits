module gui (
    input clk,

    input [6:0][0:9] score,

    input  pixel,
    input  pixel_valid,
    output pixel_ready,

    input [9:0] cursor_x,
    input [8:0] cursor_y,
    input [5:0] cursor_size,

    output        frame_buffer_write_enable,
    output [18:0] frame_buffer_write_addr,
    output        frame_buffer_data,

    input swap
);

endmodule
