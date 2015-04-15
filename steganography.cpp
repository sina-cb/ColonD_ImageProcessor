#include "steganography.h"
#include <bitset>

Steganography::Steganography()
{

}

Steganography::~Steganography()
{

}

Mat Steganography::makeEmbedImage(Mat &img, Mat &data, int bits){
    Mat res(img.size(), img.type());

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            Vec3b& color = img.at<Vec3b>(i, j);
            int r = color[0];
            int g = color[1];
            int b = color[2];

            Vec3b& color_data = data.at<Vec3b>(i, j);
            int r_data = color_data[0];
            int g_data = color_data[1];
            int b_data = color_data[2];

            bitset<8> r_bits(r_data);
            bitset<8> g_bits(g_data);
            bitset<8> b_bits(b_data);

            for (int i = 0; i < bits; i++){

                if (r_bits[7 - i] == 1){
                    r |= 1 << (bits - i - 1);
                }else{
                    r &= ~(1 << (bits - i - 1));
                }

                if (g_bits[7 - i] == 1){
                    g |= 1 << (bits - i - 1);
                }else{
                    g &= ~(1 << (bits - i - 1));
                }

                if (b_bits[7 - i] == 1){
                    b |= 1 << (bits - i - 1);
                }else{
                    b &= ~(1 << (bits - i - 1));
                }
            }

            Vec3b& res_color = res.at<Vec3b>(i, j);
            res_color[0] = r;
            res_color[1] = g;
            res_color[2] = b;
        }
    }

    return res;
}

Mat Steganography::retrieveEmbededImage(Mat &img, int bits){
    Mat res(img.size(), img.type());

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            Vec3b& color = img.at<Vec3b>(i, j);
            int r = color[0];
            int g = color[1];
            int b = color[2];

            r = r << bits;
            g = g << bits;
            b = b << bits;

            Vec3b& res_color = res.at<Vec3b>(i, j);
            res_color[0] = r;
            res_color[1] = g;
            res_color[2] = b;
        }
    }

    return res;
}

Mat Steganography::makeWhite(Mat &img, int bits){
    Mat res(img.size(), img.type());

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            Vec3b& color = img.at<Vec3b>(i, j);
            int r = color[0];
            int g = color[1];
            int b = color[2];

            int x = 1;

            for (int i = 0; i <= bits; i++){
                r ^= (-x ^ r) & (1 << i);
                g ^= (-x ^ g) & (1 << i);
                b ^= (-x ^ b) & (1 << i);
            }

            Vec3b& res_color = res.at<Vec3b>(i, j);
            res_color[0] = r;
            res_color[1] = g;
            res_color[2] = b;
        }
    }

    return res;
}

Mat Steganography::makeBlack(Mat &img, int bits){
    Mat res(img.size(), img.type());

    for(int i = 0; i < img.rows; i++)
    {
        for(int j = 0; j < img.cols; j++)
        {
            Vec3b& color = img.at<Vec3b>(i, j);
            int r = color[0];
            int g = color[1];
            int b = color[2];

            int x = 0;

            for (int i = 0; i <= bits; i++){
                r ^= (-x ^ r) & (1 << i);
                g ^= (-x ^ g) & (1 << i);
                b ^= (-x ^ b) & (1 << i);
            }

            Vec3b& res_color = res.at<Vec3b>(i, j);
            res_color[0] = r;
            res_color[1] = g;
            res_color[2] = b;
        }
    }

    return res;

}
