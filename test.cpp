//g++ -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o tst MakeFrame.h MakeFrame.cpp LiveMask.h LiveMask.cpp Calibration.h Calibration.cpp test.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_videoio

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

void testFrame(char* imgnm, char* ctrnm, char* imgcolornm, char* thresholdstr, char* countstr)
{
    cv::Mat img = imread(imgnm, CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat ctr = imread(ctrnm, CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat imgcolor = imread(imgcolornm, CV_LOAD_IMAGE_COLOR);
    
    int threshold;
    sscanf(thresholdstr, "%d", &threshold);
    
    int count;
    sscanf(countstr, "%d", &count);
    
    
    cv::Mat mask;
    cv::Mat result;
    
    clock_t t0 = clock();
    
    for(int i = 0; i < count; i++)
    {
        result = makeFrame(ctr, img, imgcolor, threshold);
    }
    
    cout << "time= " << (double)(clock() - t0) / CLOCKS_PER_SEC << "\n";
    
    
    //*
    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
    imshow( "Display window", result );
    //*/

    waitKey(0); 
}

int testCamera()
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    namedWindow("Video", CV_WINDOW_AUTOSIZE);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        imshow("Video", frame);
        if(waitKey(27) >= 0) break;
    }
    return 0;
}

int makeControl()
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    Mat frame;
    cap >> frame; // get a new frame from camera
    
    imwrite("./testimages/control.jpg", frame);
    
    return 0;
}

int testLiveMask(char* nm, char* thresholdstr)
{
    Mat fill = imread(nm, CV_LOAD_IMAGE_COLOR);
    Mat ctr = imread(nm, CV_LOAD_IMAGE_GRAYSCALE);
    
    int threshold;
    sscanf(thresholdstr, "%d", &threshold);
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    for(int i = -1; i < 0; i = waitKey(27))
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, frame, CV_BGR2GRAY);
        Mat output = makeFrame(ctr, frame, fill, threshold);
        imshow("Video", output);
        //if(waitKey(27) >= 0) break;
    }
    return 0;
}

int testLiveMaskColor(char* nm, char* fillnm, char* thresholdstr)
{
    Mat fill = imread(fillnm, CV_LOAD_IMAGE_COLOR);
    Mat ctr = imread(nm, CV_LOAD_IMAGE_COLOR);
    
    int threshold;
    sscanf(thresholdstr, "%d", &threshold);
    
    int blurkernelsize = 9;
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    for(int key = -1; key != 27; key = waitKey(10) % 256)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        //cvtColor(frame, frame, CV_BGR2GRAY);
        Mat output = makeFrameColor(ctr, frame, fill, threshold, blurkernelsize);
        imshow("Video", output);
        //if(waitKey(27) >= 0) break;
        
        if( (char)key == '=' )
        {
            threshold += 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == '-' )
        {
            threshold -= 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == 'p' )
        {
            blurkernelsize += 2;
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'o' )
        {
            blurkernelsize = max(blurkernelsize - 2, 1);
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'c' )
        {
            ctr = frame;
        }
    }
    return 0;
}

int testVideoWrite(char* nm, char* outputnm, char* thresholdstr)
{
    Mat fill = imread(nm, CV_LOAD_IMAGE_COLOR);
    Mat ctr = imread(nm, CV_LOAD_IMAGE_GRAYSCALE);
    
    int threshold;
    sscanf(thresholdstr, "%d", &threshold);
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    namedWindow("Video", 1);
    
    double w = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double h = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
    
    Size frameSize(static_cast<int>(w), static_cast<int>(h));
    
    VideoWriter output (outputnm, CV_FOURCC('H','2','6','4'), 20, frameSize, true);
    
    if ( !output.isOpened() ) //if not initialize the VideoWriter successfully, exit the program
    {
        cout << "ERROR: Failed to write the video" << endl;
        return -1;
    }
    
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, frame, CV_BGR2GRAY);
        Mat vframe = makeFrame(ctr, frame, fill, threshold);
        imshow("Video", vframe);
        output.write(vframe);
        if(waitKey(27) >= 0) break;
    }
    return 0;
}


void altTestMask(char* nm, char* thresholdstr)
{
    Mat ctr = imread(nm, CV_LOAD_IMAGE_COLOR);
    
    int threshold;
    sscanf(thresholdstr, "%d", &threshold);
    
    VideoCapture interaction(0); // open the default camera
    VideoCapture fill(0);
    
    liveMask(ctr, interaction, fill, threshold);
}

void testProcess(char* nm)
{
    VideoCapture interaction;
    VideoCapture fill;
    
    assignCameras(&interaction, &fill);
    
    int threshold = 30;
    
    Mat ctr = imread(nm, CV_LOAD_IMAGE_COLOR);
    liveMask(ctr, interaction, fill, threshold);
}

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
    testProcess(argv[1]);
    //*/
    return 0;
}





