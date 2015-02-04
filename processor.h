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
    Mat contrast_enhance(Mat& img, int lower = 0, int higher = 255);
    Mat* slicing_threshold(Mat& img, int number_of_slices);
    Mat iterative_thresholding(Mat& img, int init_threshold = 128, double diff_threshold = 1, int max_iterations = 255);
    Mat p_tile_thresholding(Mat& img, int p_tile);
    Mat adaptive_thresholding(Mat& img, int num_tiles);

};

#endif // PROCESSOR_H
