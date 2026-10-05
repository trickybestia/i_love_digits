module top_Nexys_A7_100T (
    input clk_100M,

    input ps2_clk,
    inout ps2_dat,

    output [3:0] vga_r,
    output [3:0] vga_g,
    output [3:0] vga_b,
    output       vga_hs,
    output       vga_vs
);

wire clk_sys;
wire clk_nn;
wire clk_vga;

mmcm mmcm (
    .clk_100M (clk_100M),

    .clk_sys (clk_sys),
    .clk_nn  (clk_nn),
    .clk_vga (clk_vga)
);

endmodule
