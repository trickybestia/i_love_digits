module top_Nexys_A7_100T (
    input clk_100M,

    output [3:0] vga_r,
    output [3:0] vga_g,
    output [3:0] vga_b,
    output       vga_hs,
    output       vga_vs
);

wire clk_25M175;

vga_mmcm vga_mmcm (
    .clk_100M   (clk_100M),
    .clk_25M175 (clk_25M175)
);

endmodule
