#ifndef HISTOGRAMUTILITY_H
#define HISTOGRAMUTILITY_H
#include <QImage>

#include <vector>
#include <boost/tuple/tuple.hpp>
#include "gnuplot-iostream.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cv.h"
using namespace cv;

class HistogramUtility
{
public:
    HistogramUtility();

    static QImage getHistogramImage(Mat& img);
    static void getHistogram(Mat& img, int* hist);

};

#endif // HISTOGRAMUTILITY_H
