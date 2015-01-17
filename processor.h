#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cv.h"
#include <cstdlib>
#include <time.h>
using namespace cv;

class Processor
{
public:
    Processor();

    void convert_to_grayscale(Mat& img);
    void add_noise(Mat& img, int percent);
};

#endif // PROCESSOR_H
