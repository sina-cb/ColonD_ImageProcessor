#include "histogramutility.h"

HistogramUtility::HistogramUtility()
{
}

QImage HistogramUtility::getHistogramImage(Mat& img){
    int size = 256;
    int* y = new int[size];
    for (int i = 0; i < size; i++)
        y[i] = 0;

    getHistogram(img, y);

    Gnuplot gp;
    std::vector<boost::tuple<int, int> > histogram;
    for(int i = 0; i < size; i++) {
        boost::tuple<int, int> data = boost::make_tuple(i, y[i]);
        histogram.push_back(data);
    }

    gp << "set xtics " << size / 5 << "\n";
    gp << "set ytics " << (img.cols * img.rows / 1000) << "\n";
    gp << "set format y '' \n";
    gp << "set xrange [0:" << size << "]\n";
    gp << "set terminal png size 480, 280\n";
    gp << "set output 'test.png'\n";
    gp << "plot '-' smooth freq w boxes lc rgb\"black\" notitle\n";

    gp.send1d(histogram);

    Mat image;
    image = imread("test.png", CV_LOAD_IMAGE_COLOR);
    while(!image.data){
        image = imread("test.png", CV_LOAD_IMAGE_COLOR);
    }

    switch (image.type()) {
    case CV_8UC1:
        cvtColor(image, image, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(image, image, CV_BGR2RGB);
        break;
    }
    assert(image.isContinuous());

    namedWindow("Histogram", WINDOW_AUTOSIZE);
    imshow("Histogram", image);

    if(remove("test.png") != 0)
        perror("Error deleting file");

    return QImage((uchar*) image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
}

void HistogramUtility::getHistogram(Mat &img, int *hist){
    for (int i = 0; i < 256; i++)
        hist[i] = 0;
    for(int i = 0; i < img.cols; i++)
    {
        for(int j = 0; j < img.rows; j++)
        {
            Vec3b& color = img.at<Vec3b>(Point(i, j));
            int r = color[0];
            int g = color[1];
            int b = color[2];

            if (r == g && g == b){
                hist[r]++;
            }else{
                int gray_scale = 0.21 * r + 0.72 * g + 0.07 * b;
                hist[gray_scale]++;
            }
        }
    }

}
