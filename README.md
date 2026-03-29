# AsciiConverter

A powerful C++ application that transforms images into stunning ASCII art using **OpenCV** for image processing and **Qt6** for a modern graphical user interface.

## 🚀 Features

- **Real-time Conversion:** Convert images to ASCII art with adjustable parameters.
- **GUI & CLI:** Choose between a user-friendly Qt interface or a flexible command-line tool.
- **Customizable Output:**
  - **Scaling Factor:** Resize input images to control the density of the ASCII art.
  - **Character Mapping:** Adjust character offsets to fine-tune the brightness-to-character mapping.
  - **Font Control:** Customize font face, size, thickness, and line type (e.g., Anti-aliasing).
  - **Color Options:** Output in full color (sampled from the original image) or classic black and white.
  - **Backgrounds:** Switch between light (white) and dark (black) themes.
- **High Resolution:** Generates high-quality image files as output rather than just text.

## 🛠️ Prerequisites

Before building the project, ensure you have the following installed:

- **C++17 Compiler** (GCC, Clang, or MSVC)
- **CMake** (v3.16 or higher)
- **Qt6 SDK** (Core, Gui, Widgets)
- **OpenCV** (v4.x recommended)

## 📦 Building the Project

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd ascii-art
   ```

2. **Create a build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Run CMake and build:**
   ```bash
   cmake ..
   cmake --build .
   ```

This will generate the `AsciiConverter` executable (the GUI version).

## 🖥️ Usage

### Graphical User Interface (GUI)

Run the generated `AsciiConverter` executable. 

1. **Select Source:** Click "Browse..." to pick an image file.
2. **Configure Settings:** Use the sliders and checkboxes to adjust the look of your ASCII art.
3. **Save Output:** Click "Save As" to specify where the resulting image should be saved.
4. **Generate:** Hit "Generate ASCII" to process the image and see a preview.

### Command Line Interface (CLI)

The project also includes a standalone CLI tool `image2ascii.cpp`. To compile it manually:

```bash
g++ image2ascii.cpp -o ascii_art `pkg-config --cflags --libs opencv4`
```

**CLI Usage Example:**
```bash
./ascii_art input.jpg -out result.png -s 0.2 -w -b --font-scale 0.5 -t 2
```

**Options:**
- `-s <float>`: Scaling factor (default: 0.5)
- `-o <int>`: Character offset (default: 0)
- `-w`: Use a white background
- `-b`: Use black/white font color
- `--font-scale <float>`: Scale of the font

## 🔍 How It Works

1. **Preprocessing:** The input image is resized based on the scaling factor and converted to grayscale for luminosity analysis.
2. **Character Selection:** Each pixel's brightness (0-255) is mapped to a specific character from a gradient string:
   ` .',`:_;-!liI^rv1/ftj~><L*J7T+y?)(cnus=xYVzF}{oha][kAeC4wUX3bdpqZP2EH05GSgOK96DmNR8QBWM&%#@$`
3. **Rendering:** Using OpenCV's `putText` function, the selected characters are drawn onto a new canvas. If color mode is enabled, the color of each character is sampled directly from the corresponding pixel in the original image.
