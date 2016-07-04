//g++ -g -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -o tst MakeFrame.h MakeFrame.cpp VideoFeed.h VideoFeed.cpp LiveMask.h LiveMask.cpp Calibration.h Calibration.cpp test.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -std=c++11

//TODO make cam index a parameter

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>

#include "LiveMask.h"
#include "Calibration.h"

using namespace cv;
using namespace std;

void flipvert(cv::Mat img)
{
    cv::flip(img, img, 1);
}


int main(int argc, char** argv)
{
    int cam1, cam2;
    sscanf(argv[1], "%d", &cam1);
    sscanf(argv[2], "%d", &cam2);
    
    int dwidth, dheight;
    sscanf(argv[3], "%d", &dwidth);
    sscanf(argv[4], "%d", &dheight);
    
    Size desiredres(dwidth, dheight);
    
    int offsetstep;
    sscanf(argv[5], "%d", &offsetstep);
    
    int sz;
    sscanf(argv[6], "%d", &sz);
    Size camguardsize(sz, sz);
    
    unsigned char threshold;
    sscanf(argv[7], "%u", &threshold);
    cout << "\n\n\nTHRESHOLD = " << (int)threshold << "\n\n\n";
    
    int blurkernelsize;
    sscanf(argv[8], "%d", &blurkernelsize);
    
    int offsetx, offsety;
    sscanf(argv[9],  "%d", &offsetx);
    sscanf(argv[10], "%d", &offsety);
    
    
    VideoCapture interaction;
    VideoCapture fill;
    
    assignCameras(&interaction, &fill, cam1, cam2);
    
    VideoWriter* output = NULL;
        
    if(argc == 12)
    {
        //get the width and height of frames of the video
        double w = interaction.get(CV_CAP_PROP_FRAME_WIDTH); 
        double h = interaction.get(CV_CAP_PROP_FRAME_HEIGHT);
        
        Size frameSize(static_cast<int>(w), static_cast<int>(h));
        
        output = new VideoWriter (argv[11],
                                  CV_FOURCC('H','2','6','4'),
                                  20,
                                  frameSize,
                                  true
                                 );
        
        //if the VideoWriter is not initialized correctly, exit the program
        if ( !output->isOpened() ) 
        {
            cout << "ERROR: Failed to write the video" << endl;
            return 1;
        }
    }
    
    
    liveMask(interaction,
             fill,
             desiredres,
             &flipvert,
             NULL,
             &threshold,
             &blurkernelsize,
             offsetstep,
             &offsetx,
             &offsety,
             output,
             camguardsize
            );
    
    cout << argv[0] << " "
         << cam1 << " "
         << cam2 << " "
         << dwidth << " "
         << dheight << " "
         << offsetstep << " "
         << sz << " "
         << (int)threshold << " "
         << blurkernelsize << " "
         << offsetx << " "
         << offsety << "\n"; 
    
    return 0;
}





