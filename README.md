# ASCII Art Converter

**Image and Video to ASCII Art Generator**

This project provides a powerful tool to convert images and videos into colored or black-and-white ASCII art, with a graphical user interface (GUI) and command-line support. It uses OpenCV for media processing and Tkinter for a user-friendly interface.

---

## Features

✅ Convert images to ASCII art  
✅ Convert videos to ASCII art (with audio preserved)  
✅ Customizable settings: scaling, fonts, background color, character offset, thickness, etc.  
✅ Real-time preview in the GUI  
✅ Supports multiple font styles and sizes  
✅ Dark/light theme support using `darkdetect`  
✅ Uses efficient multithreaded processing for fast conversion  
✅ Command-line utilities for batch or script-based conversion  

---

## Installation

Make sure you have **Python 3.8+** installed. Then install the dependencies:

```bash
pip install -r requirements.txt
```

**Dependencies include:**

* opencv-python
* numpy
* pillow
* sv-ttk
* darkdetect
* (optionally ffmpeg installed on your system for video processing)

---

## Usage

### GUI

Run:

```bash
python main.py
```

The GUI will open, allowing you to:

* Select an image or video
* Adjust conversion parameters
* Preview the ASCII result
* Save the generated ASCII image or video

### Command-line (Image)

```bash
python image2ascii.py <input_image> <output_image> <scaling_factor> [--charOffset N] [--white_bg True/False] [--bw_font True/False] [--fontScale 0.4] [--thickness 1] [--lineType 1] [--textOffset 1.3]
```

Example:

```bash
python image2ascii.py cat.jpg output.png 0.2 --white_bg True
```

### Command-line (Video)

```bash
python video2ascii.py
```

(You can modify the default parameters inside `video2ascii.py` or refactor to pass arguments.)

---

## How It Works

✨ **Character brightness**
`char_brightness.py` analyzes the brightness of characters to rank them from darkest to lightest for better ASCII rendering.

🖼 **Image to ASCII**
`image2ascii.py` handles scaling, grayscale conversion, and drawing ASCII characters on a blank image canvas, with optional color mapping.

🎞 **Video to ASCII**
`video2ascii.py` processes each video frame with the image2ascii pipeline and uses ffmpeg to merge audio back with the output video.

🖥 **GUI**
`main.py` provides a complete Tkinter-based interface with real-time previews and easy configuration.

---

## Project Structure

```
.
├── theme                # (azure theme files) 
├── char_brightness.py   # Character brightness ranking
├── image2ascii.py       # Image to ASCII conversion logic
├── video2ascii.py       # Video to ASCII conversion logic
├── main.py              # Tkinter GUI
├── azure.tcl            # (optional theme file)
└── requirements.txt     # Python dependencies
```

---

## Screenshots

![Light Theme](/screenshots/ss-2.png)
![Dark Theme](/screenshots/ss-1.png)

---

## Contributing

Pull requests and issues are welcome. If you have improvements, fixes, or feature requests, feel free to open an issue or PR.

---
