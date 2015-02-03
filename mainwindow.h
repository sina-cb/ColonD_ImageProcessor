#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <opencv2/opencv.hpp>

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include "projecdata.h"
#include "processor.h"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QImage mat_to_qimage(Mat& src);
    void resizeScrollArea(Mat& img);
    void updateHistogram();

    ~MainWindow();

private slots:
    void on_captureBtn_clicked();
    void on_browseBtn_clicked();

    void on_saveBtn_clicked();

    void on_convertGrayScaleBtn_clicked();

    void on_noisePercentSlider_sliderMoved(int position);

    void on_smoothBtn_clicked();

    void on_applyNoiseBtn_clicked();

    void on_medianBtn_clicked();

    void on_thresholdBinBtn_clicked();

    void on_thresholdBinSlide_sliderMoved(int position);

    void on_slicingBtn_clicked();

private:
    Ui::MainWindow *ui;
    ProjecData data;
    Processor process;

};

#endif // MAINWINDOW_H
