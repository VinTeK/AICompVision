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
	Mat prevFrame;
	if (!cam.read(prevFrame)) {
		cerr << "Cannot read a frame from video stream" << endl;
		return -1;
	}

	// looping frame by frame
	while (1)
	{
		Mat curFrame, tmpFrame, cornerFrame;

		if (!cam.read(curFrame)) {
			cerr << "Cannot read a frame from video stream" << endl;
			break;
		}

		myFrameDifferencing(prevFrame, curFrame, curFrame.clone());

		mySkinDetect(curFrame, tmpFrame);
		erode(tmpFrame, tmpFrame, Mat());
		dilate(tmpFrame, tmpFrame, Mat());

		/*=======*/

		medianBlur(tmpFrame, tmpFrame, 5);

		// grab the skin detection frame and make a copy for pip feature
		resize(tmpFrame, cornerFrame, Size(0, 0), 0.25, 0.25);
		cvtColor(cornerFrame.clone(), cornerFrame, CV_GRAY2BGR);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		// find external contours
		findContours(tmpFrame, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		for (size_t i = 0; i < contours.size(); ++i)
		{
			// convex hulls and defects for current contour
			vector<vector<Point> > pHull(1);
			vector<int>   iHull;
			vector<Vec4i> defects;
			Rect bounding = boundingRect(contours[i]);

			if (contourArea(contours[i]) > 5000)
			{
				convexHull(contours[i], pHull[0]);
				convexHull(contours[i], iHull);
				convexityDefects(contours[i], iHull, defects);

				// create a random color
				Scalar color = Scalar(rand() & 255, rand() & 255, rand() & 255);

				rectangle(curFrame, bounding, color, 4);

				//drawContours(curFrame, contours, i, color, 1, 8, hierarchy);
				drawContours(curFrame, pHull, 0, color, 2, 8);

				int digits = 0;
				// iterate through every defect for current contour
				for (size_t j = 0; j < defects.size(); ++j)
				{
					Point& srt = contours[i][defects[j][0]]; // point where defect begins
					Point& end = contours[i][defects[j][1]]; // point where defect ends
					Point& far = contours[i][defects[j][2]]; // point farthest from hull

					// eliminate unwanted defects according to finger length to hand size ratio
					int tolerence = bounding.height / 8;
					float angle = 95;
					if (ptDist(srt, far) > tolerence && ptDist(end, far) > tolerence &&
						getAngle(srt, far, end) < angle)
					{
						++digits;
						circle(curFrame, end, 5, color, 3);
					}
				}
				Scalar white = Scalar(256, 256, 256);
				if (digits == 0) {
					putText(curFrame, "fist", Point(300, 400), FONT_HERSHEY_COMPLEX, 1.5, white, 2);
				} else if (digits == 2 || digits == 3) {
					putText(curFrame, "victory", Point(300, 400), FONT_HERSHEY_COMPLEX, 1.5, white, 2);
				} else if (digits >= 4) {
					putText(curFrame, "high five", Point(300, 400), FONT_HERSHEY_COMPLEX, 1.5, white, 2);
				}
			}
			// draw the skin detection frame in the corner of the window
			Rect roi(0, 0, cornerFrame.cols, cornerFrame.rows);
			Mat subView = curFrame(roi);
			cornerFrame.copyTo(subView);

			imshow("win0", curFrame);
		}

		/*=======*/

		if (waitKey(30) == 27)
			break;

		prevFrame = curFrame;
	}

	cam.release();
	destroyAllWindows();

	return 0;
}