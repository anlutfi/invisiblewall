//gcc -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -o tst test.c -lopencv_core -lopencv_imgproc -lopencv_highgui

#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

cv::Mat makeFrame(cv::Mat control, cv::Mat interaction, cv::Mat fill, int threshold)
{
    cv::Mat diff; 
    cv::absdiff(control, interaction, diff);
    
    cv::Mat result = Mat::zeros(control.rows, control.cols, CV_8UC3);
    
    for(int i = 0; i < diff.rows; i++)
    {
        for(int j = 0; j < diff.cols; j++)
        {
            if(diff.at<uchar>(i, j) >= threshold)
            {
               result.at<Vec3b>(i, j) = fill.at<Vec3b>(i, j);
            }
        }
    }
    
    return result;
} 

cv::Mat extractDiff(cv::Mat img1, cv::Mat img2, int threshold)
{
    cv::Mat diff; 
    cv::absdiff(img1, img2, diff);
    
    cv::Mat mask = Mat::zeros(img1.rows, img1.cols, CV_8UC1);
        
    /*
    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
    imshow( "Display window", mask );                  

    waitKey(0);
    //*/
    for(int i = 0; i < img1.rows; i++)
    {
        for(int j = 0; j < img1.cols; j++)
        {
            if(diff.at<uchar>(i, j) >= threshold)
            {
               mask.at<uchar>(i, j) = 255;
            }
        }
    }
    
    return mask;
}

int main(int argc, char** argv)
{
    cv::Mat img = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat ctr = imread(argv[2], CV_LOAD_IMAGE_GRAYSCALE);
    
    int threshold;
    sscanf(argv[3], "%d", &threshold);
    
    int count;
    sscanf(argv[4], "%d", &count);
    
    
    cv::Mat mask;
    cv::Mat imgcolor = imread(argv[2], CV_LOAD_IMAGE_COLOR);
    cv::Mat result;
    clock_t t0 = clock();
    
    //*
    for(int i = 0; i < count; i++)
    {
        mask = extractDiff(img, ctr, threshold);
        imgcolor.copyTo(result, mask);
    }
    //*/
    cout << "time traditional = " << (double)(clock() - t0) / CLOCKS_PER_SEC << "\n";
    
    t0 = clock();
    
    for(int i = 0; i < count; i++)
    {
        result = makeFrame(ctr, img, imgcolor, threshold);
    }
    
    cout << "time quick = " << (double)(clock() - t0) / CLOCKS_PER_SEC << "\n";
    
    
    
    
    
    
    
    //*
    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );
    imshow( "Display window", result );
    //*/

    waitKey(0);                                          
    
    return 0;
}





