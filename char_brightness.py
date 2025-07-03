import cv2
import numpy as np
import sys

sys.stdout.reconfigure(encoding='utf-8')

# Characters to test (you can add or remove any)
chars = list("1234567890-=QWERTYUIOP[]ASDFGHJKL;'ZXCVBNM,./qwertyuiopasdfghjklzxcvbnm!@#$%^&*()_+~`{}:<>?")

# Settings
fontFace = cv2.FONT_HERSHEY_SIMPLEX
fontScale = 1
thickness = 2
img_size = (50, 50)
bg_color = 255  # White background for measuring character darkness

def get_char_brightness(char):
    # Create white image
    img = np.ones((img_size[1], img_size[0]), dtype=np.uint8) * bg_color

    # Get text size
    text_size = cv2.getTextSize(char, fontFace, fontScale, thickness)[0]
    text_x = (img_size[0] - text_size[0]) // 2
    text_y = (img_size[1] + text_size[1]) // 2

    # Put character on image
    cv2.putText(img, char, (text_x, text_y), fontFace, fontScale, (0,), thickness)

    # Calculate mean pixel intensity (lower means darker)
    brightness = np.mean(img)

    return brightness

# Measure brightness of each character
char_brightness = [(char, get_char_brightness(char)) for char in chars]

# Sort characters from darkest to brightest
sorted_chars = sorted(char_brightness, key=lambda x: x[1])

# Output result
print("".join([char for char, _ in sorted_chars]))
