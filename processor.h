#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cv.h"
#include "highgui.h"
#include <cstdlib>
#include <time.h>
using namespace cv;

class Processor
{
public:
    Processor();

    void convert_to_grayscale(Mat& img);
    Mat add_noise(Mat& img, int percent);
    Mat apply_filter(Mat& img, double** filter, int filter_w, int threshold = 256);
    Mat median_filter(Mat& img, int filter_w);
    Mat threshold_binary(Mat& img, int threshold = 128);

};

#endif // PROCESSOR_H
