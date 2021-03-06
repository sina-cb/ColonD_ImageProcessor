#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include "pyramid.h"
#include <vector>
#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include "projecdata.h"
#include "processor.h"
#include "steganography.h"

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

    void show_animated_images(Mat* images, int number_of_images);

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

    void on_Enhance_clicked();

    void on_iterativeBtn_clicked();

    void on_pTileBtn_clicked();

    void on_adaptiveBtn_clicked();

    void on_componentLabelingBtn_clicked();

    void on_invertBtn_clicked();

    void on_robertsOpBtn_clicked();

    void on_sobelOpBtn_clicked();

    void on_prewittOpBtn_clicked();

    void on_kirschOpBtn_clicked();

    void on_laplacianOpBtn_clicked();

    void on_gaussianBtn_clicked();

    void on_pyramidCreateBtn_clicked();

    void on_showPyramidBtn_clicked();

    void on_savePyramidBtn_clicked();

    void on_expandZeroOrderBtn_clicked();

    void on_expandFirstBtn_clicked();

    void on_showZeroBtn_clicked();

    void on_showFirstBtn_clicked();

    void on_saveZeroBtn_clicked();

    void on_saveFirstBtn_clicked();

    void on_blendBrowseBtn_clicked();

    void on_mkWhiteBtn_clicked();

    void on_mkBlackBtn_clicked();

    void on_embedBtn_clicked();

    void on_retrieveImgBtn_clicked();

private:
    Ui::MainWindow *ui;
    ProjecData data;
    Processor process;
    Steganography steganographer;
    Pyramid *pyramid;

};

#endif // MAINWINDOW_H
