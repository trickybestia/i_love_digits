from PIL import Image, ImageChops
import struct


def center_image(image: Image.Image) -> Image.Image:
    pixels_sum = 0
    center_of_mass_x = 0
    center_of_mass_y = 0

    for y in range(image.height):
        for x in range(image.width):
            pixel = image.getpixel((x, y)) / 255

            pixels_sum += pixel

            center_of_mass_x += pixel * x
            center_of_mass_y += pixel * y

    center_of_mass_x /= pixels_sum
    center_of_mass_y /= pixels_sum

    return ImageChops.offset(
        image,
        round(image.width / 2 - center_of_mass_x),
        round(image.height / 2 - center_of_mass_y),
    )


digit = Image.open("image.png").convert("L")

left, upper, right, lower = digit.getbbox()

width = right - left
height = lower - upper

size = max(width, height)

digit = digit.crop((left, upper, left + size, upper + size)).resize((20, 20))

image = Image.new("L", (28, 28))
image.paste(digit, (4, 4))
image = center_image(image)

image.save("preprocessed.png")

with open("image.bin", "wb") as file:
    for y in range(28):
        for x in range(28):
            pixel = image.getpixel((x, y)) / 255

            file.write(struct.pack("f", pixel))
