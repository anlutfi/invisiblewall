//gcc -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o tst test.c -lopencv_core -lopencv_imgproc -lopencv_highgui

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <time.h>

#include "MakeFrame.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    cv::Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat ctr = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat imgcolor = imread(argv[3], CV_LOAD_IMAGE_COLOR);
    
    int threshold;
    sscanf(argv[4], "%d", &threshold);
    
    int count;
    sscanf(argv[5], "%d", &count);
    
    
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
    
    return 0;
}





