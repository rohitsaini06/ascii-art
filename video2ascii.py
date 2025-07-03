import cv2
import os
from os import system as command
from os import makedirs as make_folder
from shutil import rmtree as delete_folder
from image2ascii import image2ascii

def video2ascii(source_file: str,
                output_file: str,
                scalingFactor: float,
                charOffset: int = 0,
                white_bg: bool = False,
                bw_font: bool = False,
                fontface = cv2.FONT_HERSHEY_SIMPLEX,
                fontScale: float = 0.4,
                thickness: int = 1,
                lineType: int = 1,
                textOffset: float = 1.3):

    source_path = os.path.dirname(source_file)

    cap = cv2.VideoCapture(source_file)
    source_fps = cap.get(cv2.CAP_PROP_FPS)
    total_frames = cap.get(cv2.CAP_PROP_FRAME_COUNT)


    video_path = source_path + "\\temp\\"
    make_folder(video_path, exist_ok=True)

    video_file = video_path + '\\temp.mp4'

    if not cap.isOpened():
        print("Error opening video file")
        exit()

    video_writer = None

    frame_count = 0
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        ascii_img = image2ascii(frame, 
                                scalingFactor,
                                charOffset, 
                                white_bg, 
                                bw_font,
                                fontface,
                                fontScale, 
                                thickness, 
                                lineType, 
                                textOffset)

        # Convert grayscale to BGR if needed
        if len(ascii_img.shape) == 2:
            ascii_img = cv2.cvtColor(ascii_img, cv2.COLOR_GRAY2BGR)

        # Initialize video writer once, using frame size
        if video_writer is None:
            height, width, _ = ascii_img.shape
            video_writer = cv2.VideoWriter(video_file, cv2.VideoWriter_fourcc(*'mp4v'), source_fps, (width, height))

        video_writer.write(ascii_img)
        print(f"frame {frame_count}/{total_frames} done", end='\r')
        frame_count += 1

    cap.release()
    if video_writer:
        video_writer.release()

    command(f"ffmpeg -i \"{source_file}\" -q:a 0 -map a \"{video_path}\\audio.mp3\" -y")
    command(f"ffmpeg -i \"{video_file}\" -i \"{video_path}\\audio.mp3\" -c copy \"{output_file}\" -y")


if __name__ == "__main__":
    video2ascii(
        source_file="C:\\Users\\ADMIN\\Documents\\Projects\\python\\ascii_art\\videos\\Shinchan Masala Story Movie Song - Trim.mp4", 
        output_file="C:\\Users\\ADMIN\\Documents\\Projects\\python\\ascii_art\\output\\shinchan.mp4", 
        scalingFactor=0.2,
        )