#ifndef LOADIMAGE_H
#define LOADIMAGE_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QString>

using namespace cv;

class LoadImage
{
public:
    LoadImage();
    void load(QString path, Mat& result);
};

#endif // LOADIMAGE_H
