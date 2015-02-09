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


/*void myFrameDifferencing(Mat& src, Mat& dst) {
	absdiff(src, dst);
	Mat grey = dst.clone();
	cvtColor(dst, grey, CV_BGR2GRAY);
	dst = grey > 40; // default 50
	Vec3b intensity = dst.at<Vec3b>(100, 100);
	}*/
void myFrameDifferencing(Mat& src, Mat& dst, Mat& image)
{
	//Mat image = src.clone();
	printf("frame rows %d Cols %d\n", image.rows, image.cols);

	for (int rows = 0; rows < image.rows; rows++)
	{
		for (int cols = 0; cols < image.cols; cols++)
		{
			/*  printf("BGR value %lf %lf %lf\n" , abs(prev_frame.at<cv::Vec3b>(rows,cols)[0] -
			curr_frame.at<cv::Vec3b>(rows,cols)[0]),
			abs(prev_frame.at<cv::Vec3b>(rows,cols)[1] -
			curr_frame.at<cv::Vec3b>(rows,cols)[0]),
			abs(prev_frame.at<cv::Vec3b>(rows,cols)[2] -
			curr_frame.at<cv::Vec3b>(rows,cols)[0]));
			*/
			image.at<cv::Vec3b>(rows, cols)[0] = abs(src.at<cv::Vec3b>(rows, cols)[0] -
				dst.at<cv::Vec3b>(rows, cols)[0]);
			image.at<cv::Vec3b>(rows, cols)[1] = abs(src.at<cv::Vec3b>(rows, cols)[1] -
				dst.at<cv::Vec3b>(rows, cols)[1]);
			image.at<cv::Vec3b>(rows, cols)[2] = abs(src.at<cv::Vec3b>(rows, cols)[2] -
				dst.at<cv::Vec3b>(rows, cols)[2]);
		}
	}
}


// based off of http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/hull/hull.html
void drawHull(cv::Mat& src, cv::Mat& dst)
{
	static RNG rng(12345);

	blur(src, src, Size(5, 5));

	vector<vector<Point> > contours;

	// find external contours
	findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0, 0));

	// find the convex hull object for each contour
	vector<vector<Point> > hull(contours.size());
	for (size_t i = 0; i < contours.size(); ++i)
	{
		if (contourArea(contours[i]) >= 5000)
		{
			convexHull(Mat(contours[i]), hull[i], false);
		}
	}

	//vector<Vec4i> defects;
	// find convexity defeats for each contour
	//convexityDefects(contours, hull, defects);

	// draw contours + hull results
	for (size_t i = 0; i < contours.size(); ++i)
	{
		Scalar color = Scalar(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dst, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
		drawContours(dst, hull, i, color, 2, 8, vector<Vec4i>(), 0, Point());
	}
}

