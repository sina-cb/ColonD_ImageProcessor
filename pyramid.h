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

private:
    int level;
    Mat *pyramid;
    Mat *expanded;

};

#endif // PYRAMID_H
