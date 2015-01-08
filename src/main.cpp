#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

int main(int argc, const char** argv)
{
	VideoCapture cap(0);
	Mat frame, frameCopy, image;

	if (!cap.isOpened())  // if not success, exit program
		cout << "No camera detected" << endl;

	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the  video
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;
	namedWindow("Camera Feed", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

	cout << "In capture ..." << endl;
	while (true)
	{
		Mat frame;
		bool bSuccess = cap.read(frame); // read a new frame from video
		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video file" << endl;
			break;
		}

		imshow("Camera Feed", frame); //show the frame in "MyVideo" window

		if (waitKey(30) == 32){ //27 is Esc
			cout << "Saving the image to /home/sina/Desktop/test.jpg" << endl;
			imwrite("/home/sina/Desktop/test.jpg", frame);

			break;
		}
	}

	waitKey(0);
	cvDestroyWindow("Camera Feed");

	return 0;
}
