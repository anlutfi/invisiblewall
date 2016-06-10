#include "VideoFeed.h"

VideoFeed::VideoFeed(VideoCapture* capture)
{
    this->capture = (*capture);
    this->mutexframe = new std::mutex();
}

void VideoFeed::run()
{
    while(true)
    {
        this->mutexframe->lock();
        this->capture >> this->frame;
        this->mutexframe->unlock(); 
    }
}

Mat VideoFeed::getFrame()
{
    this->mutexframe->lock();
    Mat f = this->frame;
    this->mutexframe->unlock();
    return f;
}
