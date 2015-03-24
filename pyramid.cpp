#include "pyramid.h"

#include <iostream>
#include <cstdio>
using namespace std;

Pyramid::Pyramid(int level)
{
    this->level = level;
    pyramid = new Mat[this->level];
    expanded = new Mat[this->level];
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

void Pyramid::show_all(){
    char name[100];

    for (int i = 0; i < level; i++){
        sprintf(name, "Image #%d", i);
        namedWindow(name, WINDOW_AUTOSIZE);
        imshow(name, pyramid[i]);
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
        imwrite(name, pyramid[i], compression_params);
    }
}

Pyramid::~Pyramid()
{
}

