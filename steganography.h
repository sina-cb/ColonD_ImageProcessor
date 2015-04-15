#ifndef STEGANOGRAPHY_H
#define STEGANOGRAPHY_H

#include "cv.h"
#include <boost/utility.hpp>
#include <iostream>
#include <string>
using namespace std;
using namespace cv;

class Steganography
{
public:
    Steganography();
    ~Steganography();

    Mat makeWhite(Mat& img, int bits);
    Mat makeBlack(Mat& img, int bits);
    Mat makeEmbedImage(Mat& img, Mat& data, int bits);
    Mat retrieveEmbededImage(Mat& img, int bits);

};

#endif // STEGANOGRAPHY_H
