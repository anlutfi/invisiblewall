#include "Calibration.h"

void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill)
{
    namedWindow("Camera Test", CV_WINDOW_AUTOSIZE);
    
    for(short i = 0; i < MAX_CAM; i++)
    {
        VideoCapture* cap = new VideoCapture();
        try
        {
            cap->open(i);
        }
        catch(...)
        {
            continue;
        }
        if( !cap->isOpened() )
        {
            delete cap;
            continue;
        }
        
        char key;
        for(key = ' ';
            tolower(key) != 'i' && tolower(key) != 'f' && tolower(key) != 'n';
            key = waitKey(10)
           )
        {
            Mat frame;
            (*cap) >> frame; // get a new frame from camera
            imshow("Camera Test", frame);
        }
        
        //*
        switch(key)
        {
            case 'i':
                *interaction = *cap;
                break;
            
            case 'f':
                *fill = *cap;
                break;
            
            case 'n':
                delete cap;
        }
        //*/
    }
    destroyWindow("Camera Test");
}

void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill, int i1, int i2)
{
    namedWindow("Camera Test", CV_WINDOW_AUTOSIZE);
    
    for(short i = i1; i <= i2; i++)
    {
        VideoCapture* cap = new VideoCapture();
        try
        {
            cap->open(i);
        }
        catch(...)
        {
            continue;
        }
        if( !cap->isOpened() )
        {
            delete cap;
            continue;
        }
        
        char key;
        for(key = ' ';
            tolower(key) != 'i' && tolower(key) != 'f' && tolower(key) != 'n';
            key = waitKey(10)
           )
        {
            Mat frame;
            (*cap) >> frame; // get a new frame from camera
            imshow("Camera Test", frame);
        }
        
        //*
        switch(key)
        {
            case 'i':
                *interaction = *cap;
                break;
            
            case 'f':
                *fill = *cap;
                break;
            
            case 'n':
                delete cap;
        }
        //*/
    }
    destroyWindow("Camera Test");
}
