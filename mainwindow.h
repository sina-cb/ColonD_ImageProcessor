#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "projecdata.h"
#include <QImage>
#include <QPixmap>

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

    ~MainWindow();

private slots:
    void on_captureBtn_clicked();
    void on_browseBtn_clicked();

    void on_saveBtn_clicked();

private:
    Ui::MainWindow *ui;
    ProjecData data;

};

#endif // MAINWINDOW_H
