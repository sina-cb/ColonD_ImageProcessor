#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_captureBtn_clicked();

    void on_browseBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
