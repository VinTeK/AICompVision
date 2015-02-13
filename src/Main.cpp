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
	//namedWindow("win1", CV_WINDOW_AUTOSIZE);

	// read a single frame now to have a frame to compare
	Mat prevFrame, curFrame, tmpFrame;
	if (!cam.read(prevFrame)) {
		cerr << "Cannot read a frame from video stream" << endl;
		return -1;
	}

	// loop frame by frame
	while (1)
	{
		if (!cam.read(curFrame)) {
			cerr << "Cannot read a frame from video stream" << endl;
			break;
		}

		mySkinDetect(curFrame, tmpFrame);
		erode(tmpFrame, tmpFrame, Mat());
		dilate(tmpFrame, tmpFrame, Mat());

		/*=======*/

		medianBlur(tmpFrame, tmpFrame, 5);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		// find external contours
		findContours(tmpFrame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		vector<vector<Point> > pHull(contours.size());
		vector<vector<int> >   iHull(contours.size());
		for (size_t i = 0; i < contours.size(); ++i)
		{
			convexHull(Mat(contours[i]), pHull[i]);
			convexHull(Mat(contours[i]), iHull[i]);
		}

#if 0
		for (size_t i = 0; i < contours.size(); ++i)
		{
			cout << "at the beginning" << ", " << i << endl;
			vector<vector<Point> > pHull(1);
			vector<vector<int> >   iHull(1);
			convexHull(Mat(contours[i]), pHull[0]);
			convexHull(Mat(contours[i]), iHull[0]);
			cout << "before defects" << ", " << i << endl;
			vector<Vec4i> defects;
			if (iHull[0].size() > 0)
			{
				convexityDefects(contours[i], iHull[0], defects);

				cout << contours.size() << "\t" << defects.size() << endl;
			}
			cout << "after defects" << ", " << i << endl;
#endif
			for (size_t i = 0; i < contours.size(); ++i)
			{
				vector<Vec4i> defects;
				convexityDefects(contours[i], iHull[i], defects);

				Scalar color = Scalar(rand() & 255, rand() & 255, rand() & 255);
				drawContours(curFrame, contours, i, color, 1, 8, hierarchy);
				drawContours(curFrame, pHull, i, color, 2, 8);
			}

			//cout << defects[0][0] << endl;
			//break;

			imshow("win0", curFrame);
		//}

		/*=======*/

		if (waitKey(30) == 27)
			break;

		prevFrame = curFrame;
	}

	cam.release();
	destroyAllWindows();

	return 0;
}