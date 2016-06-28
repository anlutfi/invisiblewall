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
    //cv::Mat cpy = img->clone();
    cv::flip(img, img, 1);
}

void testProcess(char* idx1, char* idx2, char* w, char* h, char* outputnm)
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
    
    int width, height;
    sscanf(w, "%d", &width);
    sscanf(h, "%d", &height);
    
    liveMask(interaction, fill, Size(width, height), &flipvert, NULL, output);
}

int main(int argc, char** argv)
{
    //*
    testProcess(argv[1], argv[2], argv[3], argv[4], argc == 6 ? argv[5] : NULL);
    //*/
    return 0;
}





