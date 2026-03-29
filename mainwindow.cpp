#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Image/Video to ASCII Converter");
    setFixedSize(1100, 400);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // --- Top Frame ---
    QGroupBox *topFrame = new QGroupBox(this);
    QHBoxLayout *topLayout = new QHBoxLayout(topFrame);
    
    topLayout->addWidget(new QLabel("Source File:"));
    sourceEntry = new QLineEdit();
    topLayout->addWidget(sourceEntry, 2);
    QPushButton *btnBrowseSource = new QPushButton("📁 Browse...");
    topLayout->addWidget(btnBrowseSource);

    topLayout->addWidget(new QLabel("Output File:"));
    outputEntry = new QLineEdit();
    topLayout->addWidget(outputEntry, 2);
    QPushButton *btnBrowseOutput = new QPushButton("💾 Save As");
    topLayout->addWidget(btnBrowseOutput);

    mainLayout->addWidget(topFrame);

    // --- Main Paned Window ---
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    
    // --- Settings Frame ---
    QGroupBox *settingsFrame = new QGroupBox("Conversion Settings");
    QFormLayout *formLayout = new QFormLayout(settingsFrame);

    // Scale
    QHBoxLayout *scaleLayout = new QHBoxLayout();
    scaleSlider = new QSlider(Qt::Horizontal);
    scaleSlider->setRange(10, 200);
    scaleSlider->setValue(100);
    scaleLabel = new QLabel("100%");
    scaleLayout->addWidget(scaleSlider);
    scaleLayout->addWidget(scaleLabel);
    formLayout->addRow("Scaling Factor:", scaleLayout);

    // Char Offset
    charOffsetSpin = new QSpinBox();
    charOffsetSpin->setRange(-100, 100);
    charOffsetSpin->setValue(0);
    formLayout->addRow("Char Offset:", charOffsetSpin);

    // Text Offset
    QHBoxLayout *textOffsetLayout = new QHBoxLayout();
    textOffsetSlider = new QSlider(Qt::Horizontal);
    textOffsetSlider->setRange(10, 20); // 1.0 to 2.0 scaled by 10
    textOffsetSlider->setValue(13);
    textOffsetLabel = new QLabel("1.30");
    textOffsetLayout->addWidget(textOffsetSlider);
    textOffsetLayout->addWidget(textOffsetLabel);
    formLayout->addRow("Text Offset:", textOffsetLayout);

    // Font
    fontCombo = new QComboBox();
    fontCombo->addItems({"FONT_HERSHEY_SIMPLEX", "FONT_HERSHEY_PLAIN", "FONT_HERSHEY_DUPLEX",
                         "FONT_HERSHEY_COMPLEX", "FONT_HERSHEY_TRIPLEX", "FONT_HERSHEY_COMPLEX_SMALL",
                         "FONT_HERSHEY_SCRIPT_SIMPLEX", "FONT_HERSHEY_SCRIPT_COMPLEX"});
    formLayout->addRow("Font:", fontCombo);

    // Font Size
    fontSizeSpin = new QDoubleSpinBox();
    fontSizeSpin->setRange(0.1, 10.0);
    fontSizeSpin->setSingleStep(0.1);
    fontSizeSpin->setValue(0.4);
    formLayout->addRow("Font Size:", fontSizeSpin);

    // Thickness
    thicknessSpin = new QSpinBox();
    thicknessSpin->setRange(1, 10);
    thicknessSpin->setValue(1);
    formLayout->addRow("Thickness:", thicknessSpin);

    // Line Type
    lineTypeSpin = new QSpinBox();
    lineTypeSpin->setRange(1, 16);
    lineTypeSpin->setValue(16); // 16 = LINE_AA in OpenCV
    formLayout->addRow("Line Type:", lineTypeSpin);

    // Checkboxes
    grayscaleCheck = new QCheckBox("Grayscale Font");
    formLayout->addRow("", grayscaleCheck);
    whiteBgCheck = new QCheckBox("White Background");
    formLayout->addRow("", whiteBgCheck);

    // Buttons
    QPushButton *btnGenerate = new QPushButton("Generate ASCII");
    QPushButton *btnReset = new QPushButton("Reset");
    formLayout->addRow(btnGenerate);
    formLayout->addRow(btnReset);

    splitter->addWidget(settingsFrame);

    // --- Preview Frame ---
    QGroupBox *previewFrame = new QGroupBox("Preview");
    QVBoxLayout *previewLayout = new QVBoxLayout(previewFrame);
    previewLabel = new QLabel("ASCII preview will appear here");
    previewLabel->setAlignment(Qt::AlignCenter);
    previewLabel->setMinimumSize(400, 300);
    previewLayout->addWidget(previewLabel);
    
    splitter->addWidget(previewFrame);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);
    
    mainLayout->addWidget(splitter);

    // --- Status Bar ---
    statusBar = new QLabel("Ready • No file loaded");
    mainLayout->addWidget(statusBar);

    setCentralWidget(centralWidget);

    // --- Connections ---
    connect(btnBrowseSource, &QPushButton::clicked, this, &MainWindow::browseSource);
    connect(btnBrowseOutput, &QPushButton::clicked, this, &MainWindow::browseOutput);
    connect(btnGenerate, &QPushButton::clicked, this, &MainWindow::generateAscii);
    connect(btnReset, &QPushButton::clicked, this, &MainWindow::resetAll);
    connect(scaleSlider, &QSlider::valueChanged, this, &MainWindow::updateScaleLabel);
    connect(textOffsetSlider, &QSlider::valueChanged, this, &MainWindow::updateTextOffsetLabel);
}

MainWindow::~MainWindow() {}

void MainWindow::updateScaleLabel(int value) {
    scaleLabel->setText(QString::number(value) + "%");
}

void MainWindow::updateTextOffsetLabel(int value) {
    textOffsetLabel->setText(QString::number(value / 10.0, 'f', 2));
}

void MainWindow::browseSource() {
    QString file = QFileDialog::getOpenFileName(this, "Select Source File", "", "Images/Videos (*.png *.jpg *.jpeg *.bmp *.mp4 *.avi *.mov)");
    if (!file.isEmpty()) {
        sourceEntry->setText(file);
        statusBar->setText("Selected file: " + file);

        cv::Mat img = cv::imread(file.toStdString());
        if (img.empty()) {
            // Might be a video
            cv::VideoCapture cap(file.toStdString());
            if (cap.isOpened()) {
                cap.read(img);
                cap.release();
            }
        }
        
        if (!img.empty()) {
            displayImagePreview(img);
        } else {
            statusBar->setText("Could not preview file.");
        }
    }
}

void MainWindow::browseOutput() {
    QString file = QFileDialog::getSaveFileName(this, "Save Output File", "", "PNG Image (*.png);;MP4 Video (*.mp4)");
    if (!file.isEmpty()) {
        outputEntry->setText(file);
        statusBar->setText("Output will be saved to: " + file);
    }
}

void MainWindow::resetAll() {
    scaleSlider->setValue(100);
    charOffsetSpin->setValue(0);
    textOffsetSlider->setValue(13);
    fontCombo->setCurrentIndex(0);
    fontSizeSpin->setValue(0.4);
    thicknessSpin->setValue(1);
    lineTypeSpin->setValue(16);
    grayscaleCheck->setChecked(false);
    whiteBgCheck->setChecked(false);
    sourceEntry->clear();
    outputEntry->clear();
    previewLabel->clear();
    previewLabel->setText("ASCII preview will appear here");
    statusBar->setText("Reset completed");
}

void MainWindow::displayImagePreview(const cv::Mat& img) {
    QImage qimg = matToQImage(img);
    if (!qimg.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(qimg);
        // Scale to fit preview area keeping aspect ratio
        previewLabel->setPixmap(pixmap.scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

QImage MainWindow::matToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_RGB888);
        return image.rgbSwapped().copy(); // Deep copy and BGR->RGB
    } else if (mat.type() == CV_8UC1) {
        QImage image(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8);
        return image.copy();
    }
    return QImage();
}

// --- Image Processing Logic ---
char MainWindow::getChar(int color, int offset, bool white_bg) {
    std::string chars = " .',`:_;-!liI^rv1/ftj~><L*J7T+y?)(cnus=xYVzF}{oha][kAeC4wUX3bdpqZP2EH05GSgOK96DmNR8QBWM&%#@$";
    chars.insert(0, offset, ' ');
    if (white_bg) { std::reverse(chars.begin(), chars.end()); }
    return chars[(color * chars.length()) / 256];
}

cv::Mat MainWindow::image2ascii(cv::Mat image, float scaling_factor, int char_offset, 
                                bool white_bg, bool bw_font, int font_face, 
                                float font_scale, int thickness, int line_type, float text_offset) {
    cv::resize(image, image, cv::Size(), scaling_factor, scaling_factor);
    int height = image.rows;
    int width = image.cols;

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);

    int baseline = 0;
    cv::Size textSize = cv::getTextSize("A", font_face, font_scale, thickness, &baseline);
    int font_padding = textSize.height * text_offset;

    cv::Mat ascii_image(height * font_padding, width * font_padding, CV_8UC3, 
                        white_bg ? cv::Scalar(255, 255, 255) : cv::Scalar(0, 0, 0));

    cv::Scalar textColor = white_bg ? cv::Scalar(0, 0, 0) : cv::Scalar(255, 255, 255);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            char text = getChar(gray_image.at<uchar>(y, x), char_offset, white_bg);
            cv::Scalar color = bw_font ? textColor : cv::Scalar(image.at<cv::Vec3b>(y, x)[0], image.at<cv::Vec3b>(y, x)[1], image.at<cv::Vec3b>(y, x)[2]);
            cv::putText(ascii_image, std::string(1, text), cv::Point(x * font_padding, y * font_padding), 
                        font_face, font_scale, color, thickness, line_type);
        }
    }
    return ascii_image;
}

void MainWindow::generateAscii() {
    if (sourceEntry->text().isEmpty() || outputEntry->text().isEmpty()) {
        statusBar->setText("Error: Source and Output files must be specified.");
        return;
    }

    cv::Mat img = cv::imread(sourceEntry->text().toStdString());
    if (img.empty()) {
        statusBar->setText("Error: Video conversion not yet implemented, or invalid image.");
        return;
    }

    statusBar->setText("Generating ASCII...");
    QApplication::processEvents(); // Keep UI responsive

    // Map combo box index to OpenCV font constant
    int fontFace = fontCombo->currentIndex(); // OpenCV font enums map perfectly 0-7

    cv::Mat result = image2ascii(
        img, 
        scaleSlider->value() / 100.0f,
        charOffsetSpin->value(),
        whiteBgCheck->isChecked(),
        grayscaleCheck->isChecked(),
        fontFace,
        fontSizeSpin->value(),
        thicknessSpin->value(),
        lineTypeSpin->value(),
        textOffsetSlider->value() / 10.0f
    );

    cv::imwrite(outputEntry->text().toStdString(), result);
    displayImagePreview(result);
    statusBar->setText("ASCII image saved successfully!");
}