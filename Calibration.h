#ifndef __CALIB_H__
#define __CALIB_H__

#include "commons.h"

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>

#include "MakeFrame.h"


#define MAX_CAM 2

/*
#define LEFT_KEY 81
#define RIGHT_KEY 83
#define UP_KEY 82
#define DOWN_KEY 84
//*/
using namespace cv;
using namespace std;

/**
void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill, int i1, int i2);


Function to assign cameras with indexes i1 and i2 to VideoCaptures interaction and fill.
The user sees the image of each camera and assigns accordingly.
*/
void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill, int i1, int i2);

void calibrate(cv::VideoCapture interaction,
               cv::VideoCapture fill,
               unsigned char* threshold,
               int* blurkernelsize,
               int* offsetx,
               int* offsety,
               int offsetstep,
               Size desiredres
              );

#endif

