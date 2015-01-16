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

    Mat& OrigImage(){return orig_img_;}
    void OrigImage(Mat& image){orig_img_ = image;}

private:
    Mat image_;
    Mat orig_img_;

};

#endif // PROJECDATA_H
