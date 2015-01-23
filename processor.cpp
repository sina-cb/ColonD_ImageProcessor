#include "processor.h"
#include <math.h>
#include <algorithm>
#include <iostream>

using namespace cv;
using namespace std;

Processor::Processor()
{
}

void Processor::convert_to_grayscale(Mat &img){
    for(int i = 0; i < img.cols; i++)
    {
        for(int j = 0; j < img.rows; j++)
        {
            Vec3b& color = img.at<Vec3b>(Point(i, j));
            int r = color[0];
            int g = color[1];
            int b = color[2];
            int gray_scale = 0.21 * r + 0.72 * g + 0.07 * b;

            color[0] = gray_scale;
            color[1] = gray_scale;
            color[2] = gray_scale;
        }
    }
}

Mat Processor::median_filter(Mat &img, int filter_w){
    int redu = filter_w / 2;

    Mat res(img.rows - redu * 2, img.cols - redu * 2, img.type());

    for(int i = redu; i < res.cols - redu; i++){
        for(int j = redu; j < res.rows - redu; j++){

            vector<int> values;
            for (int ii = 0; ii < filter_w; ii++){
                for (int jj = 0; jj < filter_w; jj++){
                    Vec3b& color = img.at<Vec3b>(Point(i + ii - redu, j + jj - redu));
                    values.push_back(color[0]);
                }
            }
            sort(values.begin(), values.end());

            Vec3b& color = res.at<Vec3b>(Point(i, j));

            int new_val = values[values.size() / 2 + 1];

            color[0] = new_val;
            color[1] = new_val;
            color[2] = new_val;
        }
    }

    return res;
}

Mat Processor::apply_filter(Mat& img, double** filter, int filter_w, int threshold){
    int redu = filter_w / 2;

    Mat res(img.rows - redu * 2, img.cols - redu * 2, img.type());

    for(int i = redu; i < res.cols - redu; i++){
        for(int j = redu; j < res.rows - redu; j++){

            double sum = 0;
            for (int ii = 0; ii < filter_w; ii++){
                for (int jj = 0; jj < filter_w; jj++){
                    Vec3b& color = img.at<Vec3b>(Point(i + ii - redu, j + jj - redu));
                    sum += (color[0] * filter[ii][jj]);
                }
            }

            Vec3b& color = res.at<Vec3b>(Point(i, j));

            if (abs(sum - img.at<Vec3b>(Point(i, j))[0]) < threshold) {
                color[0] = sum;
                color[1] = sum;
                color[2] = sum;
            }else{
                color[1] = color[0];
                color[2] = color[0];
            }
        }
    }

    return res;
}

Mat Processor::threshold_binary(Mat &img, int threshold){
    Mat res(img.size(), img.type());

    for(int i = 0; i < res.cols; i++){
        for(int j = 0; j < res.rows; j++){
            int r = img.at<Vec3b>(Point(i, j))[0];

            Vec3b& color = res.at<Vec3b>(Point(i, j));
            if (r < threshold){
                color[0] = 0;
                color[1] = 0;
                color[2] = 0;
            }else{
                color[0] = 255;
                color[1] = 255;
                color[2] = 255;
            }
        }
    }

    return res;
}

Mat Processor::add_noise(Mat &img, int percent){
    Mat res(img.size(), img.type());
    img.copyTo(res);

    srand (time(NULL));

    for(int i = 0; i < res.cols; i++){
        for(int j = 0; j < res.rows; j++){
            int r = rand() % 100;

            if (r < percent){
                Vec3b& color = res.at<Vec3b>(Point(i, j));

                if (color[0] == color[1] && color[1] == color[2]){
                    r = rand() % 256;
                    //                    color[0] += r;
                    //                    color[1] += r;
                    //                    color[2] += r;
                    color[0] = color[1] = color[2] = r;
                }else{
                    //                    color[0] += rand() % 256;
                    //                    color[1] += rand() % 256;
                    //                    color[2] += rand() % 256;
                    color[0] = rand() % 256;
                    color[1] = rand() % 256;
                    color[2] = rand() % 256;
                }
            }
        }
    }

    return res;
}
