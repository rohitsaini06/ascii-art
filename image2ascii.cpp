#include <iostream>
#include <time.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void showHelp()
{
    cout << "Usage: ./ascii_art <image_path> [options]\n\n"
         << "Options:\n"
         << "  -h, --help           Show this help message and exit\n"
         << "  -out, --output <str> Output image path (default: output_ascii.png)\n"
         << "  -s <float>           Scaling factor to resize the input image (default: 0.5)\n"
         << "  -o <int>             Character offset (default: 0)\n"
         << "  -w                   Use a white background (default: black background)\n"
         << "  -b                   Use black/white font color instead of image colors (default: colored)\n"
         << "  -f <int>             OpenCV Font face (default: 0 / FONT_HERSHEY_SIMPLEX)\n"
         << "  --font-scale <float> Scale of the font (default: 0.4)\n"
         << "  -t <int>             Thickness of the font lines (default: 1)\n"
         << "  -l <int>             Line type (default: 1 / LINE_AA)\n"
         << "  --text-offset <float> Padding/offset multiplier for text positioning (default: 1.3)\n"
         << "\nExample:\n"
         << "  ./ascii_art input.jpg -out result.png -s 0.2 -w -b --font-scale 0.5 -t 2\n";
}

char getChar(int color, int offset = 0, bool white_bg = false)
{
    string chars = " .',`:_;-!liI^rv1/ftj~><L*J7T+y?)(cnus=xYVzF}{oha][kAeC4wUX3bdpqZP2EH05GSgOK96DmNR8QBWM&%#@$";
    chars.insert(0, offset, ' ');
    if (white_bg) {
        reverse(chars.begin(), chars.end());
    }
   
    return chars[(color * chars.length()) / 256];
}

cv::Mat image2ascii(cv::Mat image, float scaling_factor, int char_offset = 0, bool white_bg = false, bool bw_font = false, cv::HersheyFonts font_face = cv::FONT_HERSHEY_SIMPLEX, float font_scale = 0.4, int thickness = 1, int line_type = 1, float text_offset = 1.3)
{
    cv::resize(image, image, cv::Size(), scaling_factor, scaling_factor);

    int height = image.rows;
    int width = image.cols;

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    int baseline = 0;
    cv::Size textSize = cv::getTextSize("A",
                                        font_face,
                                        font_scale, thickness, &baseline);

    int font_padding = textSize.height * text_offset;

    int ascii_height = height * font_padding;
    int ascii_width = width * font_padding;

    cv::Mat ascii_image;
    if (white_bg) {
        ascii_image = cv::Mat(ascii_height, ascii_width, CV_8UC3, cv::Scalar(255, 255, 255));
    } else {
        ascii_image = cv::Mat(ascii_height, ascii_width, CV_8UC3, cv::Scalar(0, 0, 0));
    }

    if (bw_font) {
        cv::Scalar textColor = white_bg ? cv::Scalar(0, 0, 0) : cv::Scalar(255, 255, 255);
        
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                char text = getChar(gray_image.at<uchar>(y, x), char_offset, white_bg);
                cv::putText(ascii_image,
                            string(1, text),
                            cv::Point(x * font_padding, y * font_padding),
                            font_face,
                            font_scale, textColor, thickness, line_type);
            }
        }
    } else {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                char text = getChar(gray_image.at<uchar>(y, x), char_offset, white_bg);
                cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
                cv::Scalar color(pixel[0], pixel[1], pixel[2]);
                cv::putText(ascii_image,
                            string(1, text),
                            cv::Point(x * font_padding, y * font_padding),
                            font_face,
                            font_scale, color, thickness, line_type);
            }
        }
    }

    return ascii_image;
}

int main(int argc, char** argv)
{
    // If no arguments passed, show help
    if (argc < 2) {
        showHelp();
        return 1;
    }

    // Default parameters
    string image_path = "";
    string output_path = "output_ascii.png"; // Added default output path
    float scaling_factor = 0.5f; 
    int char_offset = 0;
    bool white_bg = false;
    bool bw_font = false;
    int font_face = cv::FONT_HERSHEY_SIMPLEX;
    float font_scale = 0.4f;
    int thickness = 1;
    int line_type = 1; 
    float text_offset = 1.3f;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            showHelp();
            return 0;
        } else if ((arg == "-out" || arg == "--output") && i + 1 < argc) { // Added parsing for output path
            output_path = argv[++i];
        } else if (arg == "-s" && i + 1 < argc) {
            scaling_factor = atof(argv[++i]);
        } else if (arg == "-o" && i + 1 < argc) {
            char_offset = atoi(argv[++i]);
        } else if (arg == "-w") {
            white_bg = true;
        } else if (arg == "-b") {
            bw_font = true;
        } else if (arg == "-f" && i + 1 < argc) {
            font_face = atoi(argv[++i]);
        } else if (arg == "--font-scale" && i + 1 < argc) {
            font_scale = atof(argv[++i]);
        } else if (arg == "-t" && i + 1 < argc) {
            thickness = atoi(argv[++i]);
        } else if (arg == "-l" && i + 1 < argc) {
            line_type = atoi(argv[++i]);
        } else if (arg == "--text-offset" && i + 1 < argc) {
            text_offset = atof(argv[++i]);
        } else {
            // Assume the argument is the image path if it's not a known flag
            image_path = arg;
        }
    }

    if (image_path.empty()) {
        cerr << "Error: No input image provided!" << endl;
        showHelp();
        return 1;
    }

    clock_t start = clock();
    
    // Load the image
    cv::Mat img = cv::imread(image_path);
    if (img.empty()) {
        cerr << "Error: Could not read image from " << image_path << endl;
        return 1;
    }
    
    cout << "Generating ASCII art... Please wait." << endl;
    
    // Process the image
    cv::Mat result = image2ascii(
        img, 
        scaling_factor, 
        char_offset, 
        white_bg, 
        bw_font, 
        (cv::HersheyFonts)font_face, 
        font_scale, 
        thickness, 
        line_type, 
        text_offset
    );
    
    // Save using the custom output path
    cv::imwrite(output_path, result);
    cout << "Success! Saved output to: " << output_path << endl;
    
    clock_t end = clock();
    double time_taken = (double) (end - start) * 1000 / CLOCKS_PER_SEC;
    cout << "Time Taken: " << time_taken << "ms" << endl;

    return 0;
}