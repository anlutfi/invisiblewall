#ifndef __CALIB_H__
#define __CALIB_H__

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>


#define MAX_CAM 2

using namespace cv;
using namespace std;

/**
void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill, int i1, int i2);


Function to assign cameras with indexes i1 and i2 to VideoCaptures interaction and fill.
The user sees the image of each camera and assigns accordingly.
*/
void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill, int i1, int i2);

#endif

