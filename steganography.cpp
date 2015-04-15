#include "steganography.h"
#include <bitset>

Steganography::Steganography()
{

}

Steganography::~Steganography()
{

}

Mat Steganography::makeEmbedText(Mat &img, string text, int bits){
    Mat res(img.size(), img.type());

    int text_index = 0;
    int bits_index = -1;

    bitset<8> *bit_s;

    for(int i = 0; i < img.rows; i++)
    {
        if (text_index == text.length()){
            break;
        }

        for(int j = 0; j < img.cols; j++)
        {
            Vec3b& color = img.at<Vec3b>(i, j);
            int r = color[0];
            int g = color[1];
            int b = color[2];

            if (text_index == text.length()){
                break;
            }

            for (int k = 0; k < bits; k++){
                if (bits_index == -1){
                   bit_s = new bitset<8>(text[text_index]);
                   text_index++;
                   bits_index = 0;
                }

                int x = 0;
                if (bit_s[bits_index] == 1){
                    x = 1;
                }

                r ^= (-x ^ r) & (1 << k);
                g ^= (-x ^ g) & (1 << k);
                b ^= (-x ^ b) & (1 << k);

                bits_index++;

                if (bits_index == 8){
                    bits_index = -1;
                }
            }

            Vec3b& res_color = res.at<Vec3b>(i, j);
            res_color[0] = r;
            res_color[1] = g;
            res_color[2] = b;

        }
    }


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
