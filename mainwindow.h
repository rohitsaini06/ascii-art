#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browseSource();
    void browseOutput();
    void generateAscii();
    void resetAll();
    void updateScaleLabel(int value);
    void updateTextOffsetLabel(int value);

private:
    // Core processing functions embedded from your image2ascii.cpp
    char getChar(int color, int offset = 0, bool white_bg = false);
    cv::Mat image2ascii(cv::Mat image, float scaling_factor, int char_offset, 
                        bool white_bg, bool bw_font, int font_face, 
                        float font_scale, int thickness, int line_type, float text_offset);

    void displayImagePreview(const cv::Mat& img);
    QImage matToQImage(const cv::Mat& mat);

    // UI Elements
    QLineEdit *sourceEntry;
    QLineEdit *outputEntry;
    
    QSlider *scaleSlider;
    QLabel *scaleLabel;
    
    QSpinBox *charOffsetSpin;
    
    QSlider *textOffsetSlider;
    QLabel *textOffsetLabel;
    
    QComboBox *fontCombo;
    QDoubleSpinBox *fontSizeSpin;
    QSpinBox *thicknessSpin;
    QSpinBox *lineTypeSpin;
    
    QCheckBox *grayscaleCheck;
    QCheckBox *whiteBgCheck;
    
    QLabel *previewLabel;
    QLabel *statusBar;
};

#endif // MAINWINDOW_H