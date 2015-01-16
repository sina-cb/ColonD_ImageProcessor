#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cv.h"
using namespace cv;

class Processor
{
public:
    Processor();

    void convert_to_grayscale(Mat& img);
};

#endif // PROCESSOR_H
