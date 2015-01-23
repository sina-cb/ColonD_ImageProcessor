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
    void OrigImage(Mat& image){
        Mat res(image.size(), image.type());
        image.copyTo(res);
        orig_img_ = res;
    }

    Mat& ColorOrigImage(){return color_orig_img;}
    void ColorOrigImage(Mat& image){
        Mat res(image.size(), image.type());
        image.copyTo(res);
        color_orig_img = res;
    }

private:
    Mat image_;
    Mat orig_img_;
    Mat color_orig_img;

};

#endif // PROJECDATA_H
