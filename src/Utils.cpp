#include <opencv2/imgproc/imgproc.hpp>
#include "Utils.hpp"

using namespace cv;


int myMax(int a, int b, int c) {
	int m = a;
	(void)((m < b) && (m = b));
	(void)((m < c) && (m = c));
	return m;
}


int myMin(int a, int b, int c) {
	int m = a;
	(void)((m > b) && (m = b));
	(void)((m > c) && (m = c));
	return m;
}


void mySkinDetect(Mat& src, Mat& dst) {
	for (int i = 0; i < src.rows; i++){
		for (int j = 0; j < src.cols; j++){
			//For each pixel, compute the average intensity of the 3 color channels
			Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar (unsigned character)
			int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
			if ((R > 95 && G > 40 && B > 20)
				&& (myMax(R, G, B) - myMin(R, G, B) > 15)
				&& (abs(R - G) > 15) && (R > G) && (R > B)){
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}


void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst) {
	absdiff(prev, curr, dst);
	Mat gs = dst.clone();
	cvtColor(dst, gs, CV_BGR2GRAY);
	dst = gs > 50;
	Vec3b intensity = dst.at<Vec3b>(100, 100);
}


void myMotionEnergy(Vector<Mat> mh, Mat& dst) {
	Mat mh0 = mh[0];
	Mat mh1 = mh[1];
	Mat mh2 = mh[2];

	for (int i = 0; i < dst.rows; i++){
		for (int j = 0; j < dst.cols; j++){
			if (mh0.at<uchar>(i, j) == 255 || mh1.at<uchar>(i, j) == 255
				|| mh2.at<uchar>(i, j) == 255) {
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}