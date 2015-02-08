#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/opencv.hpp>
#include "Utils.hpp"

using namespace cv;
using namespace std;


int main(int argc, char** argv) {
	// open the webcam
	VideoCapture cam(0);
	if (!cam.isOpened()) {
		cerr << "Camera failed to open" << endl;
		return -1;
	}

	namedWindow("win0", CV_WINDOW_AUTOSIZE);
	namedWindow("win1", CV_WINDOW_AUTOSIZE);
	//namedWindow("win2", CV_WINDOW_AUTOSIZE);

	// read a single frame now to have a frame to compare
	Mat prevFrame;
	if (!cam.read(prevFrame)) {
		cerr << "Cannot read a frame from video stream" << endl;
		return -1;
	}

	// initialize background subtraction algorithm
	BackgroundSubtractorMOG2 bgsub(500, 16, false);

	// loop frame by frame
	while (1) {
		Mat curFrame;
		if (!cam.read(curFrame)) {
			cerr << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat tmpFrame0 = Mat::zeros(curFrame.rows, curFrame.cols, CV_8UC1); 
		Mat tmpFrame1 = Mat::zeros(curFrame.rows, curFrame.cols, CV_8UC1); 
		Mat tmpFrame2 = Mat::zeros(curFrame.rows, curFrame.cols, CV_8UC1); 

		// display this frame without any changes
		imshow("win0", curFrame);

		// display video with skin tones colored white
		mySkinDetect(curFrame, tmpFrame0);

		// highlights pixels in this frame that are different from last frame
		//myFrameDifferencing(prevFrame, curFrame, tmpFrame0);

		//bgsub.operator() (curFrame, tmpFrame1);
		erode(tmpFrame0, tmpFrame0, Mat());
		dilate(tmpFrame0, tmpFrame0, Mat());
		drawHull(tmpFrame0, tmpFrame1);

		imshow("win1", tmpFrame1);

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
	//destroyWindow("win2");

	return 0;
}