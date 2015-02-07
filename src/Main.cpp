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

	// get dims of video capture
	int camHeight = static_cast<int>(cam.get(CV_CAP_PROP_FRAME_HEIGHT));
	int camWidth = static_cast<int>(cam.get(CV_CAP_PROP_FRAME_WIDTH));

	cout << camHeight << "x" << camWidth << endl;

	// create window that shows the unchanged webcam output
	namedWindow("UnchangedOutput", CV_WINDOW_AUTOSIZE);
	//create window that shows skin detection
	namedWindow("SkinDetection", WINDOW_AUTOSIZE);
	// create window that show pixel diffs per frame
	//namedWindow("FrameDiff", WINDOW_AUTOSIZE);
	// create window that show motion energy history
	//namedWindow("MotionHistory", WINDOW_AUTOSIZE);

#if 0
	// initialize motion history vector
	vector<Mat> motionHist;
	Mat fMH1, fMH2, fMH3;
	fMH1 = Mat::zeros(camHeight, camWidth, CV_8UC1);
	fMH2 = fMH1.clone();
	fMH3 = fMH1.clone();
	motionHist.push_back(fMH1);
	motionHist.push_back(fMH2);
	motionHist.push_back(fMH3);
#endif

	// read a single frame now to have a frame to compare
	Mat prevFrame;
	if (!cam.read(prevFrame)) {
		cerr << "Cannot read a frame from video stream" << endl;
		return -1;
	}

	// loop frame by frame
	while (1) {
		Mat curFrame;
		if (!cam.read(curFrame)) {
			cerr << "Cannot read a frame from video stream" << endl;
			break;
		}
		// create a zero array of same size as video capture
		Mat frameDst = Mat::zeros(curFrame.rows, curFrame.cols, CV_8UC1); 

		// display this frame without any changes
		//imshow("UnchangedOutput", curFrame);

		// display video with skin tones colored white
		mySkinDetect(curFrame, frameDst);
		imshow("UnchangedOutput", frameDst);
		Mat hull = drawHull(frameDst, hull);
		imshow("SkinDetection", hull);

		// highlights pixels in this frame that are different from last frame
		//myFrameDifferencing(prevFrame, curFrame, frameDst);
		//imshow("FrameDiff", frameDst);

		/*
		// delete the oldest frame and add the newest frame
		motionHist.erase(motionHist.begin());
		motionHist.push_back(frameDst);
		Mat fMH = Mat::zeros(camHeight, camWidth, CV_8UC1);
		myMotionEnergy(motionHist, fMH);
		imshow("MotionHistory", fMH);
		*/

		// break loop if user hits key
		if (waitKey(30) == 27) {
			cout << "esc key is pressed by user" << endl;
			break;
		}

		prevFrame = curFrame;
	}
	waitKey(-1);

	cam.release();
	destroyWindow("MainWindow");

	return 0;
}