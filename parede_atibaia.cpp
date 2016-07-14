//g++ -g -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -o tst MakeFrame.h MakeFrame.cpp VideoFeed.h VideoFeed.cpp LiveMask.h LiveMask.cpp Calibration.h Calibration.cpp test.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -std=c++11

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

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
    int cam1 = 0;
    int cam2 = 1;
    int dwidth = 1024;
    int dheight = 768;
    Size desiredres(dwidth, dheight);
    
    int offsetstep = 5;
    int sz = 100;
    Size camguardsize(sz, sz);
    
    unsigned char threshold = 49;
    int blurkernelsize = 3;
    int offsetx = 50;
    int offsety = 80;
    
    FILE* ini = fopen("parede.ini", "r");
    if(ini != NULL)
    {
        char line[50];
        
        int len = 0;
        fgets(line, sizeof(line), ini);
        fclose(ini);
        
        sscanf(line, "%d %d %d %d %d %d %u %d %d %d",
               &cam1,
               &cam2,
               &dwidth,
               &dheight,
               &offsetstep,
               &sz,
               &threshold,
               &blurkernelsize,
               &offsetx,
               &offsety
              );
    }
    
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
         
    ini = fopen("parede.ini", "w");
    if(ini == NULL)
    {
        cout << "PArameter file missing or corrupted. Expected parede.ini\n";
        return 1;
    }
    
    fprintf(ini, "%d %d %d %d %d %d %u %d %d %d\n",
            cam1,
            cam2,
            dwidth,
            dheight,
            offsetstep,
            sz,
            threshold,
            blurkernelsize,
            offsetx,
            offsety
           );
    
    fclose(ini);
    
    return 0;
}





