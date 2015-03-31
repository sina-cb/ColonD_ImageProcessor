#include "pyramid.h"

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include "processor.h"
using namespace std;

Pyramid::Pyramid(int level)
{
    this->level = level;
    pyramid = new Mat[this->level];
    expanded_zero = new Mat[this->level];
    expanded_first = new Mat[this->level];
}

void Pyramid::create_pyramid(Mat &img){

    Mat copy_img(img.size(), img.type());
    img.copyTo(copy_img);

    switch (copy_img.type()) {
    case CV_8UC1:
        cvtColor(copy_img, copy_img, CV_GRAY2RGB);
        break;
    case CV_8UC3:
        cvtColor(copy_img, copy_img, CV_BGR2RGB);
        break;
    }

    pyramid[0] = copy_img;
    for (int l = 1; l < level; l++){
        int new_cols = copy_img.cols;
        int new_rows = copy_img.rows;
        Mat res(new_rows / 2, new_cols / 2, copy_img.type());

        for (int i = 0; i < new_cols; i++){
            for (int j = 0; j < new_rows; j++){
                Vec3b& new_pixel = res.at<Vec3b>(Point(i / 2, j / 2));

                int sum_1 = 0;
                int sum_2 = 0;
                int sum_3 = 0;

                int count = 0;

                if (i != 0){
                    sum_1 += copy_img.at<Vec3b>(Point(i - 1, j))[0];
                    sum_2 += copy_img.at<Vec3b>(Point(i - 1, j))[1];
                    sum_3 += copy_img.at<Vec3b>(Point(i - 1, j))[2];
                    count++;
                }

                if (i != new_cols - 1){
                    sum_1 += copy_img.at<Vec3b>(Point(i + 1, j))[0];
                    sum_2 += copy_img.at<Vec3b>(Point(i + 1, j))[1];
                    sum_3 += copy_img.at<Vec3b>(Point(i + 1, j))[2];
                    count++;
                }

                if (j != 0){
                    sum_1 += copy_img.at<Vec3b>(Point(i, j - 1))[0];
                    sum_2 += copy_img.at<Vec3b>(Point(i, j - 1))[1];
                    sum_3 += copy_img.at<Vec3b>(Point(i, j - 1))[2];
                    count++;
                }

                if (j != new_rows - 1){
                    sum_1 += copy_img.at<Vec3b>(Point(i, j + 1))[0];
                    sum_2 += copy_img.at<Vec3b>(Point(i, j + 1))[1];
                    sum_3 += copy_img.at<Vec3b>(Point(i, j + 1))[2];
                    count++;
                }

                sum_1 += copy_img.at<Vec3b>(Point(i, j))[0];
                sum_2 += copy_img.at<Vec3b>(Point(i, j))[1];
                sum_3 += copy_img.at<Vec3b>(Point(i, j))[2];
                count++;

                new_pixel[0] = sum_1 / count;
                new_pixel[1] = sum_2 / count;
                new_pixel[2] = sum_3 / count;
            }
        }
        copy_img = res;
        pyramid[l] = res;
    }
}

Mat Pyramid::double_zero_order_scheme(Mat &img){

    Mat res(img.rows * 2, img.cols * 2, img.type());

    for (int i = 0; i < res.cols; i++){
        for (int j = 0; j < res.rows; j++){
            Vec3b& orig_pixel = img.at<Vec3b>(Point(i / 2, j / 2));

            Vec3b& new_pixels = res.at<Vec3b>(Point(i, j));
            new_pixels[0] = orig_pixel[0];
            new_pixels[1] = orig_pixel[1];
            new_pixels[2] = orig_pixel[2];
        }
    }

    return res;
}

void Pyramid::expand_zero_order(){

    for (int i = 0; i < level; i++){
        Mat res(pyramid[i].size(), pyramid[i].type());
        pyramid[i].copyTo(res);
        for (int j = 0; j < i; j++){
            res = double_zero_order_scheme(res);
        }
        expanded_zero[i] = res;
    }

}

Mat Pyramid::double_first_order_scheme(Mat &img){
    int orig_rows = img.rows;
    int orig_cols = img.cols;
    int orig_type = img.type();

    Mat res(orig_rows * 2, orig_cols * 2, orig_type, Scalar(0));

    for (int i = 0; i < orig_cols; i++){
        for (int j = 0; j < orig_rows; j++){
            Vec3b& orig_pixel = img.at<Vec3b>(Point(i, j));

            Vec3b& new_pixels = res.at<Vec3b>(Point(i * 2, j * 2));
            new_pixels[0] = orig_pixel[0];
            new_pixels[1] = orig_pixel[1];
            new_pixels[2] = orig_pixel[2];
        }
    }

    Mat temp;

    Point anchor(1, 1);
    double delta = 0;
    int ddepth = -1;
    const int filter_width = 3;
    float filter_elements[filter_width * filter_width] = {0.25, 0.5, 0.25, 0.5, 1, 0.5, 0.25, 0.5, 0.25};
    Mat kernel(Size(filter_width, filter_width), CV_32F, filter_elements);
    filter2D(res, temp, ddepth, kernel, anchor, delta, BORDER_CONSTANT);

    return temp;
}

void Pyramid::expand_first_order(){
    for (int i = 0; i < level; i++){
        Size size = pyramid[i].size();
        int type = pyramid[i].type();

        Mat res(size, type);
        pyramid[i].copyTo(res);
        for (int j = 0; j < i; j++){
            res = double_first_order_scheme(res);
        }
        expanded_first[i] = res;
    }
}

void Pyramid::show_all(){
    char name[100];

    for (int i = 0; i < level; i++){
        sprintf(name, "Image #%d", i);
        namedWindow(name, WINDOW_AUTOSIZE);
        imshow(name, pyramid[i]);
    }
}

void Pyramid::show_zero_order(){
    char name[100];

    for (int i = 0; i < level; i++){
        sprintf(name, "Zero Order Scheme - Image #%d", i);
        namedWindow(name, WINDOW_AUTOSIZE);
        imshow(name, expanded_zero[i]);
    }
}

void Pyramid::show_first_order(){
    char name[100];

    for (int i = 0; i < level; i++){
        sprintf(name, "First Order Scheme - Image #%d", i);
        namedWindow(name, WINDOW_AUTOSIZE);
        imshow(name, expanded_first[i]);
    }
}

void Pyramid::save_all(){
    char name[100];

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);

    for (int i = 0; i < level; i++){
        sprintf(name, "/home/sina/Desktop/Image #%d", i);
        namedWindow(name, WINDOW_AUTOSIZE);
        imwrite(name, expanded_first[i], compression_params);
    }
}

Pyramid::~Pyramid()
{
}

