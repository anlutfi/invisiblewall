#ifndef __VIDEOFEED_H__
#define __VIDEOFEED_H__


#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <mutex>

using namespace cv;

class VideoFeed
{
    public:
    std::mutex* mutexframe;
    Mat frame;
    VideoCapture capture;
    void (*reorient)(Mat);
    
    void run();
    Mat getFrame();
    
    VideoFeed(VideoCapture* capture);
    VideoFeed( VideoCapture* capture, void (*reorient)(Mat) );    
};


#endif
