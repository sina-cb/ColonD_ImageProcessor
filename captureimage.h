#ifndef CAPTUREIMAGE_H
#define CAPTUREIMAGE_H

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QString>

using namespace cv;

class CaptureImage
{
public:
    CaptureImage();
    void capture(Mat& result);
};

#endif // CAPTUREIMAGE_H
