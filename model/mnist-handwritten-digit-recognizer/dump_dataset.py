from PIL import Image
from sys import argv
from os import listdir
from os.path import splitext
from pathlib import Path
from struct import unpack

dumped_images_root = Path(argv[1])
pngs_root = dumped_images_root.parent / (dumped_images_root.name + "_pngs")

pngs_root.mkdir(exist_ok=True)

for binary_image_filename in listdir(dumped_images_root):
    binary_image_path = Path(binary_image_filename)

    with open(dumped_images_root / binary_image_filename, "rb") as binary_image:
        png_image_path = pngs_root / (splitext(binary_image_path.name)[0] + ".png")

        with Image.new("L", (28, 28)) as image:
            pixels = image.load()

            for y in range(28):
                for x in range(28):
                    pixels[x, y] = int(unpack("f", binary_image.read(4))[0] * 255)

            image.save(png_image_path, "PNG")
