#include "loadimage.h"

#include <iostream>
#include <stdio.h>

using namespace std;

LoadImage::LoadImage()
{

}

void LoadImage::load(QString path, Mat& result){
    Mat image;
    image = imread(path.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);   // Read the file

    if(!(image).data){
        cout <<  "Could not open or find the image" << std::endl ;
    }else{
        result = image;
    }
}
