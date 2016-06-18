#include "VideoFeed.h"

VideoFeed::VideoFeed(VideoCapture* capture)
{
    this->capture = capture;
    this->mutexframe = new std::mutex();
    this->reorient = NULL;
}

VideoFeed::VideoFeed( VideoCapture* capture, void (*reorient)(Mat) )
{
    this->capture = capture;
    this->mutexframe = new std::mutex();
    this->reorient = reorient;
}

void VideoFeed::run()
{
    while(true)
    {
        this->mutexframe->lock();
        *(this->capture) >> this->frame;
        this->mutexframe->unlock(); 
    }
}

Mat VideoFeed::getFrame()
{
    this->mutexframe->lock();
    Mat f = this->frame.clone();
    this->mutexframe->unlock();
    
    if(this->reorient != NULL)
        this->reorient(f);
    
    return f;
}
