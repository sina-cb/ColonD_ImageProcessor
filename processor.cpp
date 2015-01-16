#include "processor.h"

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
