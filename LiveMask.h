#ifndef __LIVE_MASK_H__
#define __LIVE_MASK_H__

#include "MakeFrame.h"
#include "VideoFeed.h"
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <thread>

using namespace cv;
/*
int liveMask(cv::Mat control, cv::VideoCapture interaction, cv::VideoCapture fill, int threshold);

Function to make and display a live feed from "fill" through a mask generated by the difference
between a frame from "interaction" and the "control" image
*/
int liveMask(cv::Mat control, cv::VideoCapture interaction, cv::VideoCapture fill, int threshold);
int liveMask( cv::VideoCapture interaction,
              cv::VideoCapture fill,
              int threshold,
              void (*reorient)(cv::Mat),
              cv::VideoWriter* video
            );
int liveMask(cv::Mat control, VideoFeed interaction, VideoFeed fill, int threshold);


#endif

