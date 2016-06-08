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
        
        std::cout << "Is this the [I]nteraction camera, the [F]ill camera or [N]one? ";
        
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
        cout << "eeeee";
        //*/
    }
    destroyWindow("Camera Test");
}
