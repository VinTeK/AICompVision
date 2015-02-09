#include "stdafx.h"
#include "Utils.hpp"

using namespace cv;
using namespace std;


int main(int argc, char** argv)
{
	// open the webcam
	VideoCapture cam(0);
	if (!cam.isOpened()) {
		cerr << "Camera failed to open" << endl;
		return -1;
	}

	namedWindow("win0", CV_WINDOW_AUTOSIZE);
	namedWindow("win1", CV_WINDOW_AUTOSIZE);

	// read a single frame now to have a frame to compare
	Mat prevFrame;
	if (!cam.read(prevFrame)) {
		cerr << "Cannot read a frame from video stream" << endl;
		return -1;
	}

	// loop frame by frame
	while (1)
	{
		Mat curFrame;
		if (!cam.read(curFrame)) {
			cerr << "Cannot read a frame from video stream" << endl;
			break;
		}

		// temporary Mats to copy output onto
		Mat tmpFrame0 = Mat::zeros(curFrame.rows, curFrame.cols, CV_8UC1); 
		Mat tmpFrame1 = Mat::zeros(curFrame.rows, curFrame.cols, CV_8UC1); 
		Mat tmpFrame2 = Mat::zeros(curFrame.rows, curFrame.cols, CV_8UC1); 

		mySkinDetect(curFrame, tmpFrame0);
		erode(tmpFrame0, tmpFrame0, Mat());
		dilate(tmpFrame0, tmpFrame0, Mat());
		imshow("win0", tmpFrame0);

		drawHull(tmpFrame0, curFrame);
		imshow("win1", curFrame);

		// break loop if user hits key
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}

		prevFrame = curFrame;
	}

	cam.release();
	destroyWindow("win0");
	destroyWindow("win1");

	return 0;
}