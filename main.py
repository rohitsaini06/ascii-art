import tkinter as tk
from tkinter import ttk, filedialog
import cv2
from PIL import Image, ImageTk
from image2ascii import image2ascii
from video2ascii import video2ascii
import sv_ttk
import darkdetect

def get_filetype(filename: str) -> str:
    ext = filename.split(".")[-1].lower()
    if ext in ["png", "jpg", "jpeg", "bmp"]:
        return "img"
    elif ext in ["mp4", "avi", "mov", "mpeg", "flv", "wmv"]:
        return "vid"
    return ""

def browse_source_file():
    file = filedialog.askopenfilename(
        filetypes=[
            ("Image Files", "*.png;*.jpg;*.jpeg;*.bmp"),
            ("Video Files", "*.mp4;*.avi;*.mov;*.mpeg;*.flv;*.wmv")
        ]
    )
    if file:
        source_file_var.set(file)
        filetype = get_filetype(file)
        status_bar.config(text=f"Selected {filetype.upper()} file: {file}")

        if filetype == "img":
            img = cv2.imread(file)
            display_image_preview(img)
        elif filetype == "vid":
            cap = cv2.VideoCapture(file)
            ret, frame = cap.read()
            cap.release()
            if ret:
                display_image_preview(frame)
            else:
                status_bar.config(text="Could not preview video.")

def browse_output_file():
    file = filedialog.asksaveasfilename(
        defaultextension=".png", 
        filetypes=[("PNG Image", "*.png"), ("MP4 Video", "*.mp4")]
    )
    if file:
        output_file_var.set(file)
        status_bar.config(text=f"Output will be saved to: {file}")

def display_image_preview(cv_img):
    try:
        if cv_img is None:
            raise ValueError("No image data")
        cv_img_rgb = cv2.cvtColor(cv_img, cv2.COLOR_BGR2RGB)
        pil_image = Image.fromarray(cv_img_rgb)
        root.update_idletasks()
        w = preview_frame.winfo_width()
        h = preview_frame.winfo_height()
        pil_image.thumbnail((w, h))
        tk_img = ImageTk.PhotoImage(pil_image)
        preview_label.config(image=tk_img, text="")
        preview_label.image = tk_img
    except Exception as e:
        preview_label.config(text="Failed to load preview", image="")
        preview_label.image = None
        status_bar.config(text=f"Preview error: {e}")

def generate_ascii():
    filepath = source_file_var.get()
    output_path = output_file_var.get()
    if not filepath:
        status_bar.config(text="No source file selected")
        return
    if not output_path:
        status_bar.config(text="No output path specified")
        return
    
    filetype = get_filetype(filepath)

    scale = scale_var.get() / 100
    char_offset = char_offset_var.get()
    white_bg = white_bg_var.get()
    bw_font = grayscale_var.get()
    fontScale = font_size_var.get() or 0.4
    thickness = int(thickness_var.get()) if thickness_var.get() else 1
    lineType = int(line_type_var.get()) if line_type_var.get() else 1
    textOffset = text_offset_var.get()
    
    try:
        if filetype == "img":
            img = cv2.imread(filepath)
            ascii_img = image2ascii(
                img,
                scalingFactor=scale,
                charOffset=char_offset,
                white_bg=white_bg,
                bw_font=bw_font,
                fontface=cv2.__dict__.get(font_var.get(), cv2.FONT_HERSHEY_SIMPLEX),
                fontScale=fontScale,
                thickness=thickness,
                lineType=lineType,
                textOffset=textOffset,
            )
            cv2.imwrite(output_path, ascii_img)
            status_bar.config(text=f"ASCII image saved to {output_path}")
            display_image_preview(ascii_img)
        elif filetype == "vid":
            video2ascii(
                source_file=filepath,
                output_file=output_path,
                scalingFactor=scale,
                charOffset=char_offset,
                white_bg=white_bg,
                bw_font=bw_font,
                fontface=cv2.__dict__.get(font_var.get(), cv2.FONT_HERSHEY_SIMPLEX),
                fontScale=fontScale,
                thickness=thickness,
                lineType=lineType,
                textOffset=textOffset,
            )
            status_bar.config(text=f"ASCII video saved to {output_path}")
        else:
            status_bar.config(text="Unsupported file type.")
    except Exception as e:
        status_bar.config(text=f"Error: {e}")

def reset_all():
    scale_var.set(100)
    char_offset_var.set(0)
    text_offset_var.set(1.3)
    font_var.set("FONT_HERSHEY_SIMPLEX")
    font_size_var.set(0.4)
    thickness_var.set(1)
    line_type_var.set(1)
    custom_char_var.set("")
    grayscale_var.set(False)
    white_bg_var.set(False)
    preview_label.config(image="", text="ASCII preview will appear here")
    preview_label.image = None
    source_file_var.set("")
    output_file_var.set("")
    status_bar.config(text="Reset completed")

root = tk.Tk()
root.title("Image/Video to ASCII Converter")
root.geometry("1100x730")
style = ttk.Style()

sv_ttk.set_theme(darkdetect.theme())

# --- Top Frame ---
top_frame = ttk.Frame(root, padding=10)
top_frame.pack(fill="x")

source_file_var = tk.StringVar()
output_file_var = tk.StringVar()

# source file
ttk.Label(top_frame, text="Source File:").grid(row=0, column=0, padx=5, pady=5, sticky="e")
source_entry = ttk.Entry(top_frame, textvariable=source_file_var)
source_entry.grid(row=0, column=1, padx=5, pady=5, sticky="ew")
ttk.Button(top_frame, text="📁 Browse…", command=browse_source_file).grid(row=0, column=2, padx=5, pady=5)

# output file
ttk.Label(top_frame, text="Output File:").grid(row=0, column=3, padx=5, pady=5, sticky="e")
output_entry = ttk.Entry(top_frame, textvariable=output_file_var)
output_entry.grid(row=0, column=4, padx=5, pady=5, sticky="ew")
ttk.Button(top_frame, text="💾 Save As", command=browse_output_file).grid(row=0, column=5, padx=5, pady=5)

# spread columns
top_frame.columnconfigure(1, weight=2)
top_frame.columnconfigure(4, weight=2)

# --- Main Paned Window ---
main_pane = ttk.PanedWindow(root, orient="horizontal")
main_pane.pack(fill="both", expand=True, padx=10, pady=10)

# --- Settings Frame ---
settings_frame = ttk.Labelframe(main_pane, text="Conversion Settings", padding=15)
main_pane.add(settings_frame, weight=1)

# variables
scale_var = tk.IntVar(value=100)
char_offset_var = tk.IntVar()
text_offset_var = tk.DoubleVar(value=1.3)
font_var = tk.StringVar(value="FONT_HERSHEY_SIMPLEX")
font_size_var = tk.DoubleVar(value=0.4)
thickness_var = tk.DoubleVar(value=1)
line_type_var = tk.IntVar(value=1)
custom_char_var = tk.StringVar()
grayscale_var = tk.BooleanVar()
white_bg_var = tk.BooleanVar()

# Scaling factor with dynamic
scale_frame = ttk.Frame(settings_frame)
scale_frame.pack(fill="x", pady=3)
ttk.Label(scale_frame, text="Scaling Factor:").pack(side="left")
scale_value_label = ttk.Label(scale_frame, text=f"{scale_var.get()}%")
scale_value_label.pack(side="left", padx=5)

def update_scale_label(*args):
    scale_value_label.config(text=f"{scale_var.get()}%")
scale_var.trace_add("write", update_scale_label)

ttk.Scale(settings_frame, from_=10, to=200, variable=scale_var, orient="horizontal").pack(fill="x", pady=3)

# Char offset
ttk.Label(settings_frame, text="Char Offset").pack(anchor="w", pady=3)
ttk.Entry(settings_frame, textvariable=char_offset_var).pack(fill="x", pady=3)

# Text offset with dynamic
text_offset_frame = ttk.Frame(settings_frame)
text_offset_frame.pack(fill="x", pady=3)
ttk.Label(text_offset_frame, text="Text Offset:").pack(side="left")
text_offset_value_label = ttk.Label(text_offset_frame, text=f"{text_offset_var.get():.2f}")
text_offset_value_label.pack(side="left", padx=5)

def update_text_offset_label(*args):
    text_offset_value_label.config(text=f"{text_offset_var.get():.2f}")
text_offset_var.trace_add("write", update_text_offset_label)

ttk.Scale(settings_frame, from_=1.0, to=2.0, variable=text_offset_var, orient="horizontal").pack(fill="x", pady=3)

# font
ttk.Label(settings_frame, text="Font").pack(anchor="w", pady=3)
ttk.Combobox(settings_frame, textvariable=font_var, values=[
    "FONT_HERSHEY_SIMPLEX", "FONT_HERSHEY_PLAIN", "FONT_HERSHEY_DUPLEX",
    "FONT_HERSHEY_COMPLEX", "FONT_HERSHEY_TRIPLEX", "FONT_HERSHEY_COMPLEX_SMALL",
    "FONT_HERSHEY_SCRIPT_SIMPLEX", "FONT_HERSHEY_SCRIPT_COMPLEX"
]).pack(fill="x", pady=3)

# font size
ttk.Label(settings_frame, text="Font Size").pack(anchor="w", pady=3)
ttk.Entry(settings_frame, textvariable=font_size_var).pack(fill="x", pady=3)

# thickness
ttk.Label(settings_frame, text="Thickness").pack(anchor="w", pady=3)
ttk.Entry(settings_frame, textvariable=thickness_var).pack(fill="x", pady=3)

# line type
ttk.Label(settings_frame, text="Line Type").pack(anchor="w", pady=3)
ttk.Entry(settings_frame, textvariable=line_type_var).pack(fill="x", pady=3)

# checkboxes
ttk.Checkbutton(settings_frame, text="Grayscale", variable=grayscale_var).pack(anchor="w", pady=3)
ttk.Checkbutton(settings_frame, text="White Background", variable=white_bg_var).pack(anchor="w", pady=3)

# buttons
ttk.Button(settings_frame, text="Generate ASCII", command=generate_ascii).pack(fill="x", pady=5)
ttk.Button(settings_frame, text="Reset", command=reset_all).pack(fill="x", pady=5)

# --- Preview ---
preview_frame = ttk.Labelframe(main_pane, text="Preview", padding=15)
main_pane.add(preview_frame, weight=3)

preview_label = ttk.Label(preview_frame, text="ASCII preview will appear here", anchor="center", justify="center")
preview_label.pack(fill="both", expand=True)

# --- Status bar ---
status_bar = ttk.Label(root, text="Ready • No file loaded", anchor="w", padding=5)
status_bar.pack(side="bottom", fill="x")

root.mainloop()
