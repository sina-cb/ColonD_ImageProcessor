#include "processor.h"
#include "histogramutility.h"
#include <math.h>
#include <algorithm>
#include <iostream>
#include "boost/pending/disjoint_sets.hpp"
#include <boost/unordered/unordered_set.hpp>

using namespace cv;
using namespace std;

Processor::Processor()
{
}

Mat Processor::invert(Mat &img){
    Mat res(img.rows, img.cols, img.type());

    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            Vec3b& color = res.at<Vec3b>(Point(j, i));
            Vec3b& color_org = img.at<Vec3b>(Point(j, i));

            color[0] = 255 - color_org[0];
            color[1] = 255 - color_org[1];
            color[2] = 255 - color_org[2];
        }
    }
    return res;
}

Mat* Processor::kirsch(Mat& img){

    Mat* results = new Mat[5];

    Mat res_1(img.rows, img.cols, img.type());
    Mat res_2(img.rows, img.cols, img.type());
    Mat res_3(img.rows, img.cols, img.type());
    Mat res_4(img.rows, img.cols, img.type());

    Point anchor(1, 1);
    double delta = 0;
    int ddepth = -1;
    const int filter_width = 3;

    float elements_1[filter_width * filter_width] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    float elements_2[filter_width * filter_width] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
    float elements_3[filter_width * filter_width] = {0, 1, 1, -1, 0, 1, -1, -1, 0};
    float elements_4[filter_width * filter_width] = {1, 1, 0, 1, 0, -1, 0, -1, -1};

    Mat kernel_1(Size(filter_width, filter_width), CV_32F, elements_1);
    filter2D(img, res_1, ddepth, kernel_1, anchor, delta, BORDER_DEFAULT);

    Mat kernel_2(Size(filter_width, filter_width), CV_32F, elements_2);
    filter2D(img, res_2, ddepth, kernel_2, anchor, delta, BORDER_DEFAULT);

    Mat kernel_3(Size(filter_width, filter_width), CV_32F, elements_3);
    filter2D(img, res_3, ddepth, kernel_3, anchor, delta, BORDER_DEFAULT);

    Mat kernel_4(Size(filter_width, filter_width), CV_32F, elements_4);
    filter2D(img, res_4, ddepth, kernel_4, anchor, delta, BORDER_DEFAULT);

    Mat res(img.rows, img.cols, img.type());
    for(int i = 0; i < img.cols; i++){
        for(int j = 0; j < img.rows; j++){
            Vec3b& color1 = res_1.at<Vec3b>(Point(i, j));
            Vec3b& color2 = res_2.at<Vec3b>(Point(i, j));
            Vec3b& color3 = res_3.at<Vec3b>(Point(i, j));
            Vec3b& color4 = res_4.at<Vec3b>(Point(i, j));

            Vec3b& color_res = res.at<Vec3b>(Point(i, j));

            int max = color1[0] > color2[0] ? color1[0] : color2[0];
            max = max > color3[0] ? max : color3[0];
            max = max > color4[0] ? max : color4[0];

            color_res[0] = max;
            color_res[1] = max;
            color_res[2] = max;
        }
    }

    results[0] = res_1;
    results[1] = res_2;
    results[2] = res_3;
    results[3] = res_4;
    results[4] = res;

    return results;
}

Mat Processor::prewitt(Mat &img){
    Mat res_x(img.rows, img.cols, img.type());
    Mat res_y(img.rows, img.cols, img.type());

    Point anchor(1, 1);
    double delta = 0;
    int ddepth = -1;
    const int filter_width = 3;

    float elements_x[filter_width * filter_width] = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
    float elements_y[filter_width * filter_width] = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

    Mat kernel_x(Size(filter_width, filter_width), CV_32F, elements_x);
    filter2D(img, res_x, ddepth, kernel_x, anchor, delta, BORDER_DEFAULT);

    Mat kernel_y(Size(filter_width, filter_width), CV_32F, elements_y);
    filter2D(img, res_y, ddepth, kernel_y, anchor, delta, BORDER_DEFAULT);

    Mat res(img.rows, img.cols, img.type());
    for(int i = 0; i < img.cols; i++){
        for(int j = 0; j < img.rows; j++){
            Vec3b& color1 = res_x.at<Vec3b>(Point(i, j));
            Vec3b& color2 = res_y.at<Vec3b>(Point(i, j));

            Vec3b& color_res = res.at<Vec3b>(Point(i, j));

            int sum = sqrt(color1[0] * color1[0] + color2[0] * color2[0]);

            sum = sum > 255 ? 255:sum;
            sum = sum < 0 ? 0 : sum;

            color_res[0] = sum;
            color_res[1] = sum;
            color_res[2] = sum;
        }
    }

    return res;
}

Mat Processor::gaussian(Mat &img){
    Mat res(img.rows, img.cols, img.type());

    Point anchor(1, 1);
    double delta = 0;
    int ddepth = -1;
    const int filter_width = 7;

    float elements[filter_width * filter_width] = {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067,
                                                  0.00002292, 0.00078634, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292,
                                                  0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117,
                                                  0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771,
                                                  0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117,
                                                  0.00002292, 0.00078634, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292,
                                                  0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067
                                                  };

    Mat kernel(Size(filter_width, filter_width), CV_32F, elements);
    filter2D(img, res, ddepth, kernel, anchor, delta, BORDER_DEFAULT);

    return res;
}

Mat Processor::laplacian(Mat &img){
    Mat res(img.rows, img.cols, img.type());

    Point anchor(1, 1);
    double delta = 0;
    int ddepth = -1;
    const int filter_width = 3;

    float elements[filter_width * filter_width] = {0, 1, 0, 1, -4, 1, 0, 1, 0};

    Mat kernel(Size(filter_width, filter_width), CV_32F, elements);
    filter2D(img, res, ddepth, kernel, anchor, delta, BORDER_DEFAULT);

    int filterWidth = 3;

//    double **filter;
//    filter = new double *[filterWidth];
//    for(int i = 0; i < filterWidth; i++)
//        filter[i] = new double[filterWidth];

//    filter[0][0] = 0;
//    filter[0][1] = -1;
//    filter[0][2] = 0;

//    filter[1][0] = -1;
//    filter[1][1] = 4;
//    filter[1][2] = -1;

//    filter[2][0] = 0;
//    filter[2][1] = -1;
//    filter[2][2] = 0;

//    res = apply_filter(img, filter, filterWidth);

//    int min = 10000;
//    int max = -10000;
//    for(int i = 0; i < res.cols; i++){
//        for(int j = 0; j < res.rows; j++){
//            Vec3b& color_res = res.at<Vec3b>(Point(i, j));

//            if (color_res[0] < min){
//                min = color_res[0];
//            }

//            if (color_res[0] > max){
//                max = color_res[0];
//            }
//        }
//    }

//    for(int i = 0; i < res.cols; i++){
//        for(int j = 0; j < res.rows; j++){
//            Vec3b& color_res = res.at<Vec3b>(Point(i, j));

//            int sum = ((double)(color_res[0] - min) / (double)(max - min)) * 255;

//            color_res[0] = sum;
//            color_res[1] = sum;
//            color_res[2] = sum;
//        }
//    }

//    cout << "Max: " << max << endl;
//    cout << "Min: " << min << endl;

    for(int i = 0; i < res.cols; i++){
        for(int j = 0; j < res.rows; j++){
            Vec3b& color_res = res.at<Vec3b>(Point(i, j));

            int sum = color_res[0] + 128;

            color_res[0] = sum;
            color_res[1] = sum;
            color_res[2] = sum;
        }
    }

    return res;
}

Mat Processor::sobel(Mat &img){
    Mat res_x(img.rows, img.cols, img.type());
    Mat res_y(img.rows, img.cols, img.type());

    Point anchor(1, 1);
    double delta = 0;
    int ddepth = -1;
    const int filter_width = 3;

    float elements_x[filter_width * filter_width] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    float elements_y[filter_width * filter_width] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    Mat kernel_x(Size(filter_width, filter_width), CV_32F, elements_x);
    filter2D(img, res_x, ddepth, kernel_x, anchor, delta, BORDER_DEFAULT);

    Mat kernel_y(Size(filter_width, filter_width), CV_32F, elements_y);
    filter2D(img, res_y, ddepth, kernel_y, anchor, delta, BORDER_DEFAULT);

    Mat res(img.rows, img.cols, img.type());
    for(int i = 0; i < img.cols; i++){
        for(int j = 0; j < img.rows; j++){
            Vec3b& color1 = res_x.at<Vec3b>(Point(i, j));
            Vec3b& color2 = res_y.at<Vec3b>(Point(i, j));

            Vec3b& color_res = res.at<Vec3b>(Point(i, j));

            int sum = sqrt(color1[0] * color1[0] + color2[0] * color2[0]);

            sum = sum > 255 ? 255:sum;
            sum = sum < 0 ? 0 : sum;

            color_res[0] = sum;
            color_res[1] = sum;
            color_res[2] = sum;
        }
    }

    return res;
}

Mat Processor::roberts(Mat &img){
    Mat res_x(img.rows, img.cols, img.type());
    Mat res_y(img.rows, img.cols, img.type());

    Point anchor(0, 1);
    double delta = 0;
    int ddepth = -1;
    const int filter_width = 2;

    float elements_x[filter_width * filter_width] = {1.0, 0.0, 0.0, -1.0};
    float elements_y[filter_width * filter_width] = {0.0, 1.0, -1.0, 0.0};

    Mat kernel_x(Size(filter_width, filter_width), CV_32F, elements_x);
    filter2D(img, res_x, ddepth, kernel_x, anchor, delta, BORDER_DEFAULT);

    Mat kernel_y(Size(filter_width, filter_width), CV_32F, elements_y);
    filter2D(img, res_y, ddepth, kernel_y, anchor, delta, BORDER_DEFAULT);

    Mat res(img.rows, img.cols, img.type());
    for(int i = 0; i < img.cols; i++){
        for(int j = 0; j < img.rows; j++){
            Vec3b& color1 = res_x.at<Vec3b>(Point(i, j));
            Vec3b& color2 = res_y.at<Vec3b>(Point(i, j));

            Vec3b& color_res = res.at<Vec3b>(Point(i, j));

            int sum = sqrt(color1[0] * color1[0] + color2[0] * color2[0]);

            sum = sum > 255 ? 255:sum;
            sum = sum < 0 ? 0 : sum;

            color_res[0] = sum;
            color_res[1] = sum;
            color_res[2] = sum;
        }
    }

    return res;
}

Mat Processor::component_labeling(Mat &img, int white, int black){

    img = iterative_thresholding(img);

    int current_label = 0;

    Mat res(img.rows, img.cols, img.type());

    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            Vec3b& color = res.at<Vec3b>(Point(j, i));

            color[0] = 0;
        }
    }

    vector<int> rank(2000);
    vector<int> parent(2000);
    boost::disjoint_sets<int*, int*> ds(&rank[0], &parent[0]);
    boost::unordered_set<int> labels;

    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){

            Vec3b& color = img.at<Vec3b>(Point(j, i));
            int pixel = color[0];

            if (pixel == white){

                int pixel_top = black;
                if (i - 1 >= 0){
                    pixel_top = img.at<Vec3b>(Point(j, i - 1))[0];
                }

                int pixel_left = black;
                if (j - 1 >= 0){
                    pixel_left = img.at<Vec3b>(Point(j - 1, i))[0];
                }

                Vec3b& res_color = res.at<Vec3b>(Point(j, i));
                int result_v = -1;

                if (pixel_left == black && pixel_top == black){
                    current_label++;

                    cout << current_label << endl;

                    ds.make_set(current_label);
                    labels.insert(current_label);

                    result_v = current_label;
                }else if (pixel_left == black && pixel_top == white){

                    result_v = res.at<Vec3b>(Point(j, i - 1))[0];

                }else if (pixel_left == white && pixel_top == black){

                    result_v = res.at<Vec3b>(Point(j - 1, i))[0];

                }else if (pixel_left == white && pixel_top == white){
                    int left_label = res.at<Vec3b>(Point(j - 1, i))[0];
                    int top_label = res.at<Vec3b>(Point(j, i - 1))[0];

                    if (left_label != top_label){
                        ds.union_set(left_label, top_label);
                    }

                    result_v = left_label < top_label ? left_label : top_label;

                }

                if (result_v == -1){
                    cout << "ERROR" << endl;
                    exit(-1);
                }

                res_color[0] = result_v;
                res_color[1] = result_v;
                res_color[2] = result_v;
            }

        }
    }

    ds.normalize_sets(labels.begin(), labels.end());
    cout << "Number of components: " << ds.count_sets(labels.begin(), labels.end()) << endl;


    int r[] = {0, 150, 175, 200, 225, 250, 25, 50, 75, 100, 125};
    int g[] = {125, 150, 175, 200, 225, 250, 0, 25, 50, 75, 100};
    int b[] = {0, 25, 50, 175, 200, 225, 150, 250, 75, 100, 125};

    map<int, int> final_eq;
    int index = 0;
    int area = 0;
    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            Vec3b& color = res.at<Vec3b>(Point(j, i));
            int temp = (int)color[0];

            temp = ds.find_set(temp);

            if (color[0] != 0){
                if (final_eq[temp] == 0){
                    index++;
                    final_eq[temp] = index;
                }

                int index = (final_eq[temp] * 13) % 11;

                color[0] = r[index];
                color[1] = g[index];
                color[2] = b[index];

                area++;
            }else{
                color[0] = black;
                color[1] = black;
                color[2] = black;
            }
        }
    }

    cout << "Number of components: " << index << endl;
    cout << "Average area of the components: " << (double)(area) / index << endl;

    return res;

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

Mat Processor::apply_even_filter(Mat& img, double** filter, int filter_w){
    Mat res(img.rows, img.cols, img.type());

    for(int i = 0; i < res.cols; i++){
        for(int j = 0; j < res.rows; j++){

            double sum = 0;
            for (int ii = 0; ii < filter_w; ii++){
                for (int jj = 0; jj < filter_w; jj++){
                    Vec3b& color = img.at<Vec3b>(Point(i + ii, j + jj));
                    sum += (color[0] * filter[ii][jj]);
                }
            }

            Vec3b& color = res.at<Vec3b>(Point(i, j));

            color[0] = sum;
            color[1] = sum;
            color[2] = sum;
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

Mat Processor::contrast_enhance(Mat& img, int lower , int higher){

    int CLT[256];

    for (int i = 0; i < 256; i++){
        if (i < lower){
            CLT[i] = 0;
        }else if(i > higher){
            CLT[i] = 255;
        }else{
            CLT[i] = (int)(((double)i - (double)lower) / ((double)higher - (double)lower) * 255);
        }
    }

    Mat res(img.size(), img.type());
    for(int i = 0; i < res.cols; i++){
        for(int j = 0; j < res.rows; j++){
            int r = img.at<Vec3b>(Point(i, j))[0];
            Vec3b& color = res.at<Vec3b>(Point(i, j));

            color[0] = CLT[r];
            color[1] = CLT[r];
            color[2] = CLT[r];
        }
    }

    return res;
}

Mat Processor::adaptive_thresholding(Mat& img, int num_tiles){
    Mat res(img.size(), img.type());

    for (int i = 0; i < num_tiles; i++){
        for (int j = 0; j < num_tiles; j++){

            Range rows_r(i * img.rows / num_tiles, (i + 1) * img.rows / num_tiles);
            Range cols_r(j * img.cols / num_tiles, (j + 1) * img.cols / num_tiles);

            Mat small = img(rows_r, cols_r);
            small = iterative_thresholding(small);
            Mat small_part = res(rows_r, cols_r);

            small.copyTo(small_part);
        }
    }

    return res;
}

Mat Processor::p_tile_thresholding(Mat& img, int p_tile){
    int* hist = new int[256];
    HistogramUtility histUtil;
    histUtil.getHistogram(img, hist);

    int pixel_count = (int)((100 - p_tile) / 100.0 * img.cols * img.rows);

    int count = 0;
    int index = 0;
    while(count < pixel_count){
        count += hist[index];
        index++;
    }

    Mat res = threshold_binary(img, index);
    return res;
}

Mat Processor::iterative_thresholding(Mat& img, int init_threshold, double diff_threshold, int max_iterations){

    double diff = 256;
    int counter = 0;
    int current_threshold = init_threshold;
    while (diff > diff_threshold && counter < max_iterations){
        init_threshold = current_threshold;

        double sum1 = 0;
        double sum2 = 0;
        int count1 = 0;
        int count2 = 0;

        for(int i = 0; i < img.cols; i++){
            for(int j = 0; j < img.rows; j++){
                int r = img.at<Vec3b>(Point(i, j))[0];

                if (r < init_threshold){
                    sum1 += r;
                    count1++;
                }else{
                    sum2 += r;
                    count2++;
                }
            }
        }

        if (count1 != 0 && count2 != 0){
            sum1 /= count1;
            sum2 /= count2;
        }else{
            if (count1 == 0){
                sum1 = 0;
                sum2 /= count2;
            }else{
                sum1 /= count1;
                sum2 = 0;
            }
        }

        current_threshold = (int) ((1.0 / 2.0) * (sum1 + sum2));
        diff = abs(current_threshold - init_threshold);
        counter++;
    }

    Mat res = threshold_binary(img, current_threshold);
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

Mat* Processor::slicing_threshold(Mat& img, int number_of_slices){
    HistogramUtility histUtil;
    int* hist = new int[256];
    histUtil.getHistogram(img, hist);

    Mat* results = new Mat[number_of_slices];
    for (int i = 0; i < number_of_slices; i++){
        int range_start = (int)(i * (256.0 / number_of_slices));
        int range_end = (int)((i + 1) * (256.0 / number_of_slices));

        Mat res(img.size(), img.type());

        for(int i = 0; i < res.cols; i++){
            for(int j = 0; j < res.rows; j++){
                int r = img.at<Vec3b>(Point(i, j))[0];

                Vec3b& color = res.at<Vec3b>(Point(i, j));
                if (r > range_start && r <= range_end){
                    color[0] = 255;
                    color[1] = 255;
                    color[2] = 255;
                }else{
                    color[0] = 0;
                    color[1] = 0;
                    color[2] = 0;
                }
            }
        }

        results[i] = res;

    }

    return results;
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
                    //color[0] += r;
                    //color[1] += r;
                    //color[2] += r;
                    color[0] = color[1] = color[2] = r;
                }else{
                    //color[0] += rand() % 256;
                    //color[1] += rand() % 256;
                    //color[2] += rand() % 256;
                    color[0] = rand() % 256;
                    color[1] = rand() % 256;
                    color[2] = rand() % 256;
                }
            }
        }
    }

    return res;
}
