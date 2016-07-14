//g++ -g -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -o tst MakeFrame.h MakeFrame.cpp VideoFeed.h VideoFeed.cpp LiveMask.h LiveMask.cpp Calibration.h Calibration.cpp test.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio -std=c++11

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
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
    
    VideoCapture interaction;
    VideoCapture fill;
    
    assignCameras(&interaction, &fill, cam1, cam2);
    
    unsigned char threshold = 100;
    int blurkernelsize = 1;
    int offsetx = 0;
    int offsety = 0;
    
    ifstream inif(string(argv[0]) + string(".ini"));
    string line
    getline(inif, line);
    threshold = stoi(line);
    getline(inif, line);
    blurkernelsize = stoi(line);
    getline(inif, line);
    offsetx = stoi(line);
    getline(inif, line);
    offsety = stoi(line);
    
    inif.close();
    
    cout << "\n\n" << (int)threshold << "  " << blurkernelsize << "  " << offsetx << "  " << offsety << "\n";
    
    return 0;
    
    VideoWriter* output = NULL;
        
    if(argc == 8)
    {
        //get the width and height of frames of the video
        double w = interaction.get(CV_CAP_PROP_FRAME_WIDTH); 
        double h = interaction.get(CV_CAP_PROP_FRAME_HEIGHT);
        
        Size frameSize(static_cast<int>(w), static_cast<int>(h));
        
        output = new VideoWriter (argv[7],
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
             output,
             camguardsize
            );
    
    return 0;
}





