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


float ptDist(const Point& a, const Point& b)
{
	float d = sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
	return d;
}


float getAngle(const Point& s, const Point& f, const Point& e) 
{
	float l1 = ptDist(f, s);
	float l2 = ptDist(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / 3.14159265;
	return angle;
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
	Mat gs = dst.clone();
	cvtColor(dst, gs, CV_BGR2GRAY);
	dst = gs > 40;
	Vec3b intensity = dst.at<Vec3b>(100, 100);

	int B = intensity[0], G = intensity[1], R = intensity[2];
	if (B > 200 && G > 200 && R > 200)
	{
		putText(curr, "Slap!", Point(0, 400), FONT_HERSHEY_COMPLEX, 1.5, Scalar(256, 256, 256), 2);
		imwrite("slap.jpg", curr);
	}
}


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

