#include "stdafx.h"
#include "Utils.hpp"

using namespace cv;


int myMax(int a, int b, int c)
{
	int m = a;
	(void)((m < b) && (m = b));
	(void)((m < c) && (m = c));
	return m;
}


int myMin(int a, int b, int c)
{
	int m = a;
	(void)((m > b) && (m = b));
	(void)((m > c) && (m = c));
	return m;
}


void mySkinDetect(Mat& src, Mat& dst)
{
	dst = Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			//For each pixel, compute the average intensity of the 3 color channels
			Vec3b intensity = src.at<Vec3b>(i, j); //Vec3b is a vector of 3 uchar
			int B = intensity[0]; int G = intensity[1]; int R = intensity[2];
			if ((R > 95 && G > 40 && B > 20)
				&& (myMax(R, G, B) - myMin(R, G, B) > 15)
				&& (abs(R - G) > 15) && (R > G) && (R > B))
			{
				dst.at<uchar>(i, j) = 255;
			}
		}
	}
}


void myFrameDifferencing(Mat& prev, Mat& curr, Mat& dst)
{
	absdiff(prev, curr, dst);
	Mat grey = dst.clone();
	cvtColor(dst, grey, CV_BGR2GRAY);
	dst = grey > 40; // default 50
	Vec3b intensity = dst.at<Vec3b>(100, 100);
}


// based off of http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/hull/hull.html
void drawHull(cv::Mat& src, cv::Mat& dst)
{
	medianBlur(src, src, 5);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// find external contours
	findContours(src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	for (size_t i = 0; i < hierarchy[i][0]; ++i)
	{
		vector<vector<Point> > hull(1);
		if (contourArea(contours[i]) >= 5000)
		{
			convexHull(Mat(contours[i]), hull[0]);
		}
		Scalar color = Scalar(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, 0, color, 1, 8, hierarchy);
		//drawContours(dst, hull, 0, color, 2, 8);
	}
}

