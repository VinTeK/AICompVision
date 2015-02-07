#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
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

	cout << cam.get(CV_CAP_PROP_FRAME_HEIGHT) << "x" << cam.get(CV_CAP_PROP_FRAME_WIDTH) << endl;

	namedWindow("MainWindow", CV_WINDOW_AUTOSIZE);

	namedWindow("MyVideo0", WINDOW_AUTOSIZE);
	Mat frame0;

	// read a new frame from video
	if (!cam.read(frame0)) {
		cerr << "Cannot read a frame from video stream" << endl;
	}

	//show the frame in "MyVideo" window
	imshow("MyVideo0", frame0);

	//create a window called "MyVideo"
	namedWindow("MyVideo", WINDOW_AUTOSIZE);
	namedWindow("MyVideoMH", WINDOW_AUTOSIZE);

	vector<Mat> myMotionHistory;
	Mat fMH1, fMH2, fMH3;
	fMH1 = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);
	fMH2 = fMH1.clone();
	fMH3 = fMH1.clone();
	myMotionHistory.push_back(fMH1);
	myMotionHistory.push_back(fMH2);
	myMotionHistory.push_back(fMH3);

	while (1) {
		// read frame by frame
		Mat frame;
		if (!cam.read(frame)) {
			cerr << "Cannot read a frame from video stream" << endl;
			break;
		}

		//Returns a zero array of same size as src mat, and of type CV_8UC1
		Mat frameDest = Mat::zeros(frame.rows, frame.cols, CV_8UC1); 

		mySkinDetect(frame, frameDest);

		//myFrameDifferencing(frame0, frame, frameDest);
		imshow("MyVideo", frameDest);
		myMotionHistory.erase(myMotionHistory.begin());
		myMotionHistory.push_back(frameDest);
		Mat myMH = Mat::zeros(frame0.rows, frame0.cols, CV_8UC1);

		myMotionEnergy(myMotionHistory, myMH);

		imshow("MyVideoMH", myMH); //show the frame in "MyVideo" window
		frame0 = frame;

		// break loop if user hits key
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}

	cam.release();
	destroyWindow("MainWindow");

	return 0;
}