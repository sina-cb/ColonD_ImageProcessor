#ifndef PROJECDATA_H
#define PROJECDATA_H

#include "cv.h"
using namespace cv;

class ProjecData
{
public:
    ProjecData();

    Mat& Image(){return image_;}
    void Image(Mat& image){image_ = image;}

private:
    Mat image_;

};

#endif // PROJECDATA_H
