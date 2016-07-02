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
/*
void testProcess(char* idx1, char* idx2, char* w, char* h, char* offsetstepnm, char* outputnm)
{
    VideoCapture interaction;
    VideoCapture fill;
    
    int i1, i2;
    sscanf(idx1, "%d", &i1);
    sscanf(idx2, "%d", &i2);
    
    assignCameras(&interaction, &fill, i1, i2);
    
    VideoWriter* output = NULL;
        
    if(outputnm != NULL)
    {
        double w = interaction.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
        double h = interaction.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
        
        Size frameSize(static_cast<int>(w), static_cast<int>(h));
        
        output = new VideoWriter (outputnm, CV_FOURCC('H','2','6','4'), 20, frameSize, true);
        
        if ( !output->isOpened() ) //if not initialize the VideoWriter successfully, exit the program
        {
            cout << "ERROR: Failed to write the video" << endl;
            return;
        }
    }
    
    Mat ctr;
    interaction >> ctr;
    
    int dwidth, dheight;
    sscanf(w, "%d", &dwidth);
    sscanf(h, "%d", &dheight);
    
    int offsetstep;
    sscanf(offsetstepnm, "%d", &offsetstep);
    
    liveMask(interaction, fill, Size(dwidth, dheight), &flipvert, NULL, 100, 1, offsetstep, offsetstep, output);
}
//*/

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
    
    VideoCapture interaction;
    VideoCapture fill;
    
    assignCameras(&interaction, &fill, cam1, cam2);
    
    unsigned char threshold = 100;
    int blurkernelsize = 1;
    int offsetx = 0;
    int offsety = 0;
    
    calibrate(interaction,
              fill,
              &threshold,
              &blurkernelsize,
              &offsetx,
              &offsety,
              offsetstep,
              desiredres
             ); 
    
    VideoWriter* output = NULL;
        
    if(argc == 7)
    {
        //get the width and height of frames of the video
        double w = interaction.get(CV_CAP_PROP_FRAME_WIDTH); 
        double h = interaction.get(CV_CAP_PROP_FRAME_HEIGHT);
        
        Size frameSize(static_cast<int>(w), static_cast<int>(h));
        
        output = new VideoWriter (argv[6],
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
             threshold,
             blurkernelsize,
             offsetstep,
             offsetx,
             offsety,
             output
            );
    
    return 0;
}





