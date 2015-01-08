#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "captureimage.h"
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_captureBtn_clicked()
{
    CaptureImage win;
}



void MainWindow::on_browseBtn_clicked()
{
    QString selFilter="JPG image files (*.jpg)";
    QFileDialog::getOpenFileUrl(this, "Load image file", QDir::currentPath(), "JPG image files (*.jpg);;All files (*.*)", &selFilter);
}
