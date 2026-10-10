from dataclasses import dataclass

import PIL.Image

CURSOR_X_MAX = 639
CURSOR_Y_MAX = 479
PAINT_POS_MAX = 479
CURSOR_SIZE_MAX = 63


@dataclass
class Inputs:
    score: list[int]
    pixels: list[bool]
    cursor_x: int
    cursor_y: int
    cursor_size: int


def draw_gui(inputs: Inputs) -> PIL.Image.Image:
    result = PIL.Image.new("1", (CURSOR_X_MAX + 1, CURSOR_Y_MAX + 1))

    for i, pixel in enumerate(inputs.pixels):
        result.putpixel(
            (i % (PAINT_POS_MAX + 1), i // (PAINT_POS_MAX + 1)), pixel
        )

    return result


image = PIL.Image.new("1", (160, 480))

for i in range(11):
    start_y = i * 47
    end_y = start_y + 7

    image.dr
