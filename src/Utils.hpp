#pragma once

#include <opencv2/core/core.hpp>


/**
Returns the maximum of 3 integers.
@param a first integer
@param b second integer
@param c third integer
*/
int myMax(int a, int b, int c);

/**
Returns the minimum of 3 integers.
@param a first integer
@param b second integer
@param c third integer
*/
int myMin(int a, int b, int c);

/**
Detects whether a pixel belongs to the skin based on RGB values.
@param src The source color image
@param dst The destination grayscale image where skin pixels are colored white and the rest are
colored black
*/
void mySkinDetect(cv::Mat& src, cv::Mat& dst);

/**
Does frame differencing between the current frame and the previous frame.
@param src The current color image
@param prev The previous color image
@param dst The destination grayscale image where pixels are colored white if the corresponding
pixel intensities in the current and previous image are not the same
*/
void myFrameDifferencing(cv::Mat& prev, cv::Mat& curr, cv::Mat& dst);

/**
Accumulates the frame differences within 3 frames.
@param mh Vector of frame difference images
@param dst The destination grayscale image storing the accumulation of the frame difference images
*/
void myMotionEnergy(cv::Vector<cv::Mat> mh, cv::Mat& dst);

/**
Returns a Mat with a convex hull on top of src.
@param src The source color image
*/
cv::Mat drawHull(cv::Mat& src);