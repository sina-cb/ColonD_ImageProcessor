#ifndef PYRAMID_H
#define PYRAMID_H

#include <opencv2/opencv.hpp>
using namespace cv;

class Pyramid
{
public:
    Pyramid(int level);
    ~Pyramid();

    void create_pyramid(Mat& img);
    void show_all();
    void save_all();

    void expand_zero_order();
    void expand_first_order();

    void show_zero_order();
    void show_first_order();

private:
    int level;
    Mat *pyramid;
    Mat *expanded_zero;
    Mat *expanded_first;

    Mat double_zero_order_scheme(Mat& img);
    Mat double_first_order_scheme(Mat& img);

};

#endif // PYRAMID_H
