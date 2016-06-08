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

void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill);
void calibrateResolution(cv::VideoCapture source);

#endif

