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

void testProcess(char* idx1, char* idx2, char* outputnm)
{
    VideoCapture interaction;
    VideoCapture fill;
    
    int i1, i2;
    sscanf(idx1, "%d", &i1);
    sscanf(idx2, "%d", &i2);
    
    assignCameras(&interaction, &fill, i1, i2);
    
    /*
    fill.set(CAP_PROP_FRAME_WIDTH,  1920);
    fill.set(CAP_PROP_FRAME_HEIGHT, 1080);
    
    interaction.set(CAP_PROP_FRAME_WIDTH,  1920);
    interaction.set(CAP_PROP_FRAME_HEIGHT, 1080);
    //*/    
    
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
    liveMask(interaction, fill, Size(1920, 1080), 80, &flipvert, output);
}

/*
void testMultiProcess(char* idx1, char* idx2)
{
    VideoCapture interaction;
    VideoCapture fill;
    
    int index1, index2;
    
    sscanf(idx1, "%d", &index1);
    sscanf(idx2, "%d", &index2);
    
    
    assignCameras(&interaction, &fill, index1, index2);
    
    VideoFeed interactionfeed(&interaction);
    VideoFeed fillfeed(&fill);
    
    cout << "fill: " << fill.get(CAP_PROP_FRAME_WIDTH) << " " << fill.get(CAP_PROP_FRAME_HEIGHT) << "\n";
    cout << "interaction: " << interaction.get(CAP_PROP_FRAME_WIDTH) << " " << interaction.get(CAP_PROP_FRAME_HEIGHT) << "\n";
            
    int threshold = 30;
    
    Mat ctr;
    interaction >> ctr;
    liveMask(interactionfeed, fillfeed, threshold);
}
//*/

int main(int argc, char** argv)
{
    /*
    testFrame(argv[1], argv[2], argv[3], argv[4], argv[5]);                                    
    //*/
    
    /*
    testCamera();
    //*/
    
    /*
    makeControl();
    //*/
    
    /*
    testLiveMask(argv[1], argv[2]);
    //*/
    
    /*
    testLiveMaskColor(argv[1], argv[2], argv[3]);
    //*/
    
    /*
    testVideoWrite(argv[1], argv[2], argv[3]);
    //*/
    
    /*
    altTestMask(argv[1], argv[2]);
    //*/
    
    //*
    testProcess(argv[1], argv[2], argc == 4? argv[3] : NULL);
    //*/
    
    /*
    testMultiProcess(argv[1], argv[2]);
    //*/
    
    
    return 0;
}





