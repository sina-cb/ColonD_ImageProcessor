#include "captureimage.h"
#include <iostream>
#include <stdio.h>


using namespace std;

CaptureImage::CaptureImage()
{

}

void CaptureImage::capture(Mat& result){
    VideoCapture cap(0);
    if (!cap.isOpened())  // if not success, exit program
        cout << "No camera detected" << endl;

    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the  video
    cout << "Frame size :  " << dWidth << " x " << dHeight << endl;
    namedWindow("Camera Feed", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    cout << "In capture ..." << endl;
    Mat frame;
    while (true)
    {
        bool bSuccess = cap.read(frame); // read a new frame from video
        if (!bSuccess) //if not success, break loop
        {
            cout << "Cannot read a frame from video file" << endl;
            break;
        }

        flip(frame, frame, 1);
        imshow("Camera Feed", frame); //show the frame in "MyVideo" window

        unsigned int a = waitKey(10);
        if (a == 1048608 || a == 32){
            cout << "Image captured" << endl;
            break;
        }

        if (a == 1048603 || a == 27){
            cout << "Cancelling capture" << endl;
            break;
        }
    }

    cvDestroyWindow("Camera Feed");

    result = frame;
}
