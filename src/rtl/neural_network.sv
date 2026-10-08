// At this point, just count bright pixels and treat brightness as score
module neural_network #(
    parameter PIXELS_COUNT = 480 * 480
) (
    input clk,

    input  pixel,
    input  pixel_valid,
    output pixel_ready,

    output [6:0] score,
    output       score_valid,
    input        score_ready
);

localparam PIXELS_COUNTER_WIDTH = $clog2(PIXELS_COUNT);

enum {WORK, SCORE} state;
logic [PIXELS_COUNTER_WIDTH - 1:0] pixels_counter;
logic [PIXELS_COUNTER_WIDTH - 1:0] pixels_sum;
logic [3:0] score_index;

assign pixel_ready = state == WORK;

assign score       = pixels_sum * 100 / PIXELS_COUNT;
assign score_valid = state == SCORE;

initial begin
    state          <= WORK;
    pixels_counter <= 0;
    pixels_sum     <= 0;
    score_index    <= 0;
end

// state
always_ff @(posedge clk) begin
    case (state)
        WORK: begin
            if (pixels_counter == PIXELS_COUNT - 1 && pixel_valid) begin
                state <= SCORE;
            end
        end
        SCORE: begin
            if (score_index == 9 && score_ready) begin
                state <= WORK;
            end
        end
    endcase
end

// pixels_counter
always_ff @(posedge clk) begin
    if (pixel_valid && pixel_ready) begin
        pixels_counter <= pixels_counter == PIXELS_COUNT - 1 ? 0 : pixels_counter + 1;
    end
end

// pixels_sum
always_ff @(posedge clk) begin
    if (state == SCORE && score_index == 9 && score_ready) begin
        pixels_sum <= 0;
    end else if (pixel_valid && pixel_ready) begin
        pixels_sum <= pixels_sum + pixel;
    end
end

// score_index
always_ff @(posedge clk) begin
    if (state == SCORE && score_ready) begin
        score_index <= score_index == 9 ? 0 : score_index + 1;
    end
end

endmodule
