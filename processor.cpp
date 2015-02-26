#include "processor.h"
#include "histogramutility.h"
#include <math.h>
#include <algorithm>
#include <iostream>

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

Mat Processor::roberts(Mat &img){
    int filterWidth = 3;

    double **filter;
    filter = new double *[filterWidth];
    for(int i = 0; i < filterWidth; i++)
        filter[i] = new double[filterWidth];

    filter[0][0] = 0;   filter[0][1] = 0;   filter[0][2] = 0;
    filter[1][0] = 0;   filter[1][1] = 1;   filter[1][2] = 0;
    filter[2][0] = 0;   filter[2][1] = 0;   filter[2][2] = -1;
    Mat first = apply_filter(img, filter, filterWidth);

    filter[0][0] = 0;   filter[0][1] = 0;   filter[0][2] = 0;
    filter[1][0] = 0;   filter[1][1] = 0;   filter[1][2] = 1;
    filter[2][0] = 0;   filter[2][1] = -1;   filter[2][2] = 0;
    Mat second = apply_even_filter(img, filter, filterWidth);

    Mat res(first.rows, first.cols, first.type());

    for(int i = 0; i < img.cols - 1; i++){
        for(int j = 1; j < img.rows; j++){
            Vec3b& color1 = first.at<Vec3b>(Point(i, j - 1));
            Vec3b& color2 = second.at<Vec3b>(Point(i, j - 1));

            Vec3b& color_res = res.at<Vec3b>(Point(i, j - 1));

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

    map<int, int> equivalents;

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

                if (pixel_left == black && pixel_top == black){
                    current_label++;

                    res_color[0] = current_label;
                    res_color[1] = current_label;
                    res_color[2] = current_label;
                }else if (pixel_top == white && pixel_left == black){
                    res_color[0] = res.at<Vec3b>(Point(j, i - 1))[0];
                    res_color[1] = res.at<Vec3b>(Point(j, i - 1))[1];
                    res_color[2] = res.at<Vec3b>(Point(j, i - 1))[2];
                }else if (pixel_left == white && pixel_top == black){
                    res_color[0] = res.at<Vec3b>(Point(j - 1, i))[0];
                    res_color[1] = res.at<Vec3b>(Point(j - 1, i))[1];
                    res_color[2] = res.at<Vec3b>(Point(j - 1, i))[2];
                }else if (pixel_left == white && pixel_top == white){
                    int left_label = res.at<Vec3b>(Point(j - 1, i))[0];
                    int top_label = res.at<Vec3b>(Point(j, i - 1))[0];

                    if (top_label != left_label){

                        if (equivalents[top_label] != left_label && equivalents[left_label] != top_label){
                            equivalents[top_label] = left_label;
                        }

                    }

                    res_color[0] = left_label;
                    res_color[1] = left_label;
                    res_color[2] = left_label;
                }
            }

        }
    }

    for (int i = current_label; i > 0; i--){
        if (equivalents[i] != 0){
            vector<int> need_update;
            need_update.push_back(i);

            int temp = equivalents[i];
            while(equivalents[temp] != 0){
                if(std::find(need_update.begin(), need_update.end(), temp) == need_update.end()) {
                    need_update.push_back(temp);
                    temp = equivalents[temp];
                }else{
                    temp = *(std::min(need_update.begin(), need_update.end()));
                    break;
                }
            }

            for (size_t i = 0; i < need_update.size(); i++){
                equivalents[need_update[i]] = temp;
            }
        }
    }

    map<int, int> final_eq;
    int index = 0;
    int area = 0;
    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            Vec3b& color = res.at<Vec3b>(Point(j, i));
            int temp = (int)color[0];

            if (equivalents[temp] != 0){
                temp = equivalents[temp];
            }

            if (color[0] != 0){
                if (final_eq[temp] == 0){
                    index++;
                    final_eq[temp] = index;
                }
                color[0] = color[1] = color[2] = final_eq[temp] + 100;

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
