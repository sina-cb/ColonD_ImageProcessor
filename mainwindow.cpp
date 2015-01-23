#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadimage.h"
#include "captureimage.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include "histogramutility.h"
#include "cv.h"
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->imageLbl = new QLabel;
    ui->imageLbl->setBackgroundRole(QPalette::Base);
    ui->imageLbl->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->imageLbl->setScaledContents(true);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(ui->imageLbl);

    ui->saveBtn->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_captureBtn_clicked()
{
    CaptureImage cap;
    Mat img;
    cap.capture(img);

    switch (img.type()) {
    case CV_8UC1:
        cvtColor(img, img, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(img, img, CV_BGR2RGB);
        break;
    }
    assert(img.isContinuous());

    data.Image(img);
    data.OrigImage(img);
    data.ColorOrigImage(img);

    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(img)));
    ui->captureBtn->setEnabled(false);
    ui->saveBtn->setEnabled(true);

    updateHistogram();
}


void MainWindow::on_browseBtn_clicked()
{
    QString selFilter="JPG image files (*.jpg)";
    QUrl url = QFileDialog::getOpenFileUrl(this, "Load image file", QDir::currentPath(), "JPG image files (*.jpg);;All files (*.*)", &selFilter);

    LoadImage load;
    Mat img;
    load.load(url.path(), img);

    switch (img.type()) {
    case CV_8UC1:
        cvtColor(img, img, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(img, img, CV_BGR2RGB);
        break;
    }
    assert(img.isContinuous());

    data.Image(img);
    data.OrigImage(img);
    data.ColorOrigImage(img);

    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(img)));
    ui->captureBtn->setEnabled(false);
    ui->saveBtn->setEnabled(true);

    updateHistogram();
}

void MainWindow::updateHistogram(){
    HistogramUtility::getHistogramImage(data.Image());
}

void MainWindow::resizeScrollArea(Mat& img){
    double scrollWidth = 810;
    double scrollHeight = 460;

    double width = img.cols;
    double height = img.rows;

    double scale = 1;

    if (width > scrollWidth){
        scale = scrollWidth / width;

        width *= scale;
        height *= scale;
    }

    if (height > scrollHeight){
        scale = scrollHeight / height;

        width *= scale;
        height *= scale;
    }

    ui->scrollArea->resize(width, height);
}

QImage MainWindow::mat_to_qimage(Mat& img)
{
    resizeScrollArea(img);
    return QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
}


void MainWindow::on_saveBtn_clicked()
{
    QString selFilter="JPG image files (*.jpg)";
    QString fileName = QFileDialog::getSaveFileName(this, "Save file", QDir::currentPath(), "JPG image files (*.jpg);;All files (*.*)", &selFilter);

    if (!fileName.endsWith(".jpg")){
        fileName = fileName + ".jpg";
    }

    Mat temp;
    cvtColor(data.Image(), temp, CV_RGB2BGR);

    imwrite(fileName.toStdString().c_str(), temp);
}

void MainWindow::on_convertGrayScaleBtn_clicked()
{
    process.convert_to_grayscale(data.Image());
    process.convert_to_grayscale(data.OrigImage());
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}

void MainWindow::on_noisePercentSlider_sliderMoved(int position)
{
    Mat noisy = process.add_noise(data.Image(), position);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(noisy)));
    updateHistogram();
}

void MainWindow::on_smoothBtn_clicked()
{
    int filterWidth = ui->filterWidthTxt->text().toInt();

    double **filter;
    filter = new double *[filterWidth];
    for(int i = 0; i < filterWidth; i++)
        filter[i] = new double[filterWidth];

    for (int i = 0; i < filterWidth; i++){
        for (int j = 0; j < filterWidth; j++){
            filter[i][j] = 1.0 / (filterWidth * filterWidth);
        }
    }

    Mat smoothed = process.apply_filter(data.Image(), filter, filterWidth);
    data.Image(smoothed);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}

void MainWindow::on_medianBtn_clicked()
{
    Mat smoothed = process.median_filter(data.Image(), ui->medianFilterWidthTxt->text().toInt());
    data.Image(smoothed);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}

void MainWindow::on_applyNoiseBtn_clicked()
{
    Mat noisy = process.add_noise(data.Image(), ui->noisePercentSlider->value());
    data.Image(noisy);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}


void MainWindow::on_thresholdBinBtn_clicked()
{
    Mat binary = process.threshold_binary(data.Image(), ui->thresholdBinSlide->value());
    data.Image(binary);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}

void MainWindow::on_thresholdBinSlide_sliderMoved(int position)
{
    Mat binary = process.threshold_binary(data.Image(), position);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(binary)));
    updateHistogram();
}
