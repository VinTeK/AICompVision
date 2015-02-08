#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>
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
			Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar
			int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
			if ((R > 95 && G > 40 && B > 20)
				&& (myMax(R, G, B) - myMin(R, G, B) > 15)
				&& (abs(R - G) > 15) && (R > G) && (R > B)) {

				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}


void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst) {
	absdiff(prev, curr, dst);
	Mat grey = dst.clone();
	cvtColor(dst, grey, CV_BGR2GRAY);
	dst = grey > 30; // default 50
	Vec3b intensity = dst.at<Vec3b>(100, 100);
}


void myMotionEnergy(Vector<Mat> mh, Mat& dst) {
	Mat mh0 = mh[0];
	Mat mh1 = mh[1];
	Mat mh2 = mh[2];

	for (int i = 0; i < dst.rows; i++){
		for (int j = 0; j < dst.cols; j++){
			if (mh0.at<uchar>(i, j) == 255
				|| mh1.at<uchar>(i, j) == 255
				|| mh2.at<uchar>(i, j) == 255) {

				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}


// lifted from http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/hull/hull.html
Mat drawHull(cv::Mat& src) {
	static RNG rng(12345);

	blur(src, src, Size(3, 3));

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// Find external contours
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	// Find the convex hull object for each contour
	vector<vector<Point> > hull(contours.size());
	for (size_t i = 0; i < contours.size(); ++i) {
		convexHull(Mat(contours[i]), hull[i], false);
	}

	// Draw contours + hull results
	Mat drawing = Mat::zeros(src.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); ++i) {
		Scalar color = Scalar(rand() & 255, rand() & 255, rand() & 255);
		//drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(drawing, hull, i, color, 2, 8, vector<Vec4i>(), 0, Point());
	}

	return drawing;
}

