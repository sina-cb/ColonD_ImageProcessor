#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDir>
#include <QDebug>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
#include "loadimage.h"
#include "captureimage.h"
#include "histogramutility.h"


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

void MainWindow::on_slicingBtn_clicked()
{
    int num_slices = ui->slicerSlide->value();

    Mat* results = process.slicing_threshold(data.Image(), num_slices);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);

    QString dirPath = QFileDialog::getExistingDirectory(this, "Select a folder", QDir::currentPath(), QFileDialog::ShowDirsOnly);

    for (int i = 0; i < num_slices; i++){
        QString file_path = QString("%1/sliced_image #%2.jpg").arg(dirPath, QString::number(i + 1));
        imwrite(file_path.toStdString().c_str(), results[i], compression_params);
    }

    boost::thread thread(&MainWindow::show_animated_images, this, results, num_slices);

}

void MainWindow::show_animated_images(Mat *images, int number_of_images){
    namedWindow("Sliced Images", CV_WINDOW_FREERATIO);
    resizeWindow("Sliced Images", 640, 480);
    int i = 0;
    int rounds = 0;
    while(rounds < 2){
        imshow("Sliced Images", images[i]);

        boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        i++;
        if (i == number_of_images){
            i = 0;
            rounds++;
        }
    }
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

    Mat temp(data.Image().rows, data.Image().cols, CV_8UC1);
    cvtColor(data.Image(), temp, CV_RGB2BGR);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);

    imwrite(fileName.toStdString().c_str(), temp, compression_params);
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

void MainWindow::on_Enhance_clicked()
{
    int lowerBound = ui->contrastLowerTxt->text().toInt();
    int higherBound = ui->contrastHigherTxt->text().toInt();

    Mat enhanced = process.contrast_enhance(data.Image(), lowerBound, higherBound);
    data.Image(enhanced);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}

void MainWindow::on_iterativeBtn_clicked()
{
    Mat binary = process.iterative_thresholding(data.Image());
    data.Image(binary);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}

void MainWindow::on_adaptiveBtn_clicked()
{
    int num_tiles = ui->adaptiveThresholdTxt->text().toInt();

    Mat binary = process.adaptive_thresholding(data.Image(), num_tiles);
    data.Image(binary);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}

void MainWindow::on_pTileBtn_clicked()
{
    int p_tile = ui->pTileTxt->text().toInt();

    Mat binary = process.p_tile_thresholding(data.Image(), p_tile);
    data.Image(binary);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();

}

void MainWindow::on_componentLabelingBtn_clicked()
{
    Mat components = process.component_labeling(data.Image());
    data.Image(components);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
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

void MainWindow::on_invertBtn_clicked()
{
    Mat invert = process.invert(data.Image());
    data.Image(invert);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(invert)));
    updateHistogram();
}

void MainWindow::on_robertsOpBtn_clicked()
{
    Mat roberts = process.roberts(data.Image());
    data.Image(roberts);
    ui->imageLbl->setPixmap(QPixmap::fromImage(mat_to_qimage(data.Image())));
    updateHistogram();
}
