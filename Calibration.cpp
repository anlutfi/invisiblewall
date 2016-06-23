#include "Calibration.h"

void assignCameras(cv::VideoCapture* interaction, cv::VideoCapture* fill, int i1, int i2)
{
    VideoCapture* caps[MAX_CAM];
    caps[0] = new VideoCapture();
    caps[1] = new VideoCapture();
    
    try
    {
        caps[0]->open(i1);
        caps[1]->open(i2);
    }
    catch(...)
    {
    }
    
    for(unsigned char i = 0; i < MAX_CAM; i++)
        if( !caps[i]->isOpened() )
        {
            delete caps[i];
            continue;
        }
    
    namedWindow("Camera Test", CV_WINDOW_AUTOSIZE);
    
    for(unsigned char i = 0; i < MAX_CAM; i++)
    {
        VideoCapture* cap = caps[i];
        char key;
        for(key = ' ';
            tolower(key) != 'i' && tolower(key) != 'f' && tolower(key) != 'n';
            key = waitKey(10)
           )
        {
            Mat frame;
            (*cap) >> frame;
            imshow("Camera Test", frame);
        }
        
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
    }
    destroyWindow("Camera Test");
}
