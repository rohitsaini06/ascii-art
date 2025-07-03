import cv2
import numpy as np
from math import floor
from functools import lru_cache
from concurrent.futures import ThreadPoolExecutor
import argparse
from time import monotonic

@lru_cache(maxsize=None)
def getChar(chars: str, color: int) -> str:
    return chars[floor((color / 256) * len(chars))]

def image2ascii(image,
                scalingFactor: float,
                charOffset: int = 0,
                white_bg: bool = False,
                bw_font: bool = False,
                fontface = cv2.FONT_HERSHEY_SIMPLEX,
                fontScale: float = 0.4,
                thickness: int = 1,
                lineType: int = 1,
                textOffset: float = 1.3):

    # Consistent font face
    fontFace = cv2.FONT_HERSHEY_SIMPLEX

    # Character set based on background
    # chars = "@B8W#ZQOUCXokdpmzcvunxrjft/1{]?-+~<i!I;:."[::-1] + " " * charOffset
    chars = "$@#%&MWBQ8RNmD69KOgSG50HE2PZqpdb3XUw4CeAk[]aho{}FzVYx=sunc()?y+T7J*L<>~jtf/1vr^Iil!-;_:`,'."[::-1] + " " * charOffset
    if white_bg:
        chars = chars[::-1]  # Optional: reverse again if needed for white background

    # Resize image to shrink to ASCII scale
    image = cv2.resize(src=image, dsize=None, fx=scalingFactor, fy=scalingFactor)
    height, width, _ = image.shape

    # Convert to grayscale
    grayscale_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Calculate font padding based on character size
    char_size = cv2.getTextSize(".", fontFace, fontScale, thickness)[0]
    fontPadding = int(max(char_size) * textOffset)

    # Create blank ASCII image canvas
    ascii_height, ascii_width = height * fontPadding, width * fontPadding
    ascii_image = np.zeros((ascii_height, ascii_width, 3), np.uint8)

    # Fill background white if needed
    if white_bg:
        ascii_image.fill(255)

    # Define rendering functions
    def process_row_bwfont_whitebg(i):
        for j in range(width):
            color = grayscale_image[i, j]
            cv2.putText(ascii_image, getChar(chars, color),
                        (j * fontPadding, i * fontPadding),
                        fontFace, fontScale, (0, 0, 0), thickness, lineType)

    def process_row_bwfont_blackbg(i):
        for j in range(width):
            color = grayscale_image[i, j]
            cv2.putText(ascii_image, getChar(chars, color),
                        (j * fontPadding, i * fontPadding),
                        fontFace, fontScale, (255, 255, 255), thickness, lineType)

    def process_row_colorfont_blackbg(i):
        for j in range(width):
            color = grayscale_image[i, j]
            pixel = image[i, j].tolist()
            cv2.putText(ascii_image, getChar(chars, color),
                        (j * fontPadding, i * fontPadding),
                        fontFace, fontScale, pixel, thickness, lineType)

    # Use threads for speed
    with ThreadPoolExecutor() as executor:
        if bw_font:
            if white_bg:
                executor.map(process_row_bwfont_whitebg, range(height))
            else:
                executor.map(process_row_bwfont_blackbg, range(height))
        else:
            executor.map(process_row_colorfont_blackbg, range(height))

    return ascii_image


if __name__ == '__main__':


    parser = argparse.ArgumentParser()
    parser.add_argument("image", type=str, help='complete path of image including its name.')
    parser.add_argument("outputpath", type=str, help='output path')
    parser.add_argument("scalingFactor", type=float, help='scaling factor of original image')
    parser.add_argument("--charOffset", type = int, default=0, help='number of extra blank spaces')
    parser.add_argument("--white_bg", type=bool, default=0, help='true/false')
    parser.add_argument("--bw_font", type=bool, default=0, help='true/false')
    parser.add_argument("--fontScale", type=int, default=1)
    parser.add_argument("--thickness", type=int, default=1)
    parser.add_argument("--lineType", type=int, default=1)
    parser.add_argument("--textOffset", type=float, default=1.3, help='space btw two characters')
    args = parser.parse_args()


    st = monotonic()
    img = image2ascii(cv2.imread(args.image), args.scalingFactor, args.charOffset, args.white_bg, args.bw_font, args.fontScale, args.thickness, args.lineType, args.textOffset)
    cv2.imwrite(args.outputpath, img)
    et = monotonic()
    print(f"Time Taken : {et-st}sec.")