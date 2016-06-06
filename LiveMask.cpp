#include "LiveMask.h"

int liveMask(cv::Mat control, cv::VideoCapture interaction, cv::VideoCapture fill, int threshold)
{
    Mat fillframe;
    
    if( !interaction.isOpened() || !fill.isOpened() )  // check if we succeeded
        return -1;
    
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    for(int i = -1; i < 0; i = waitKey(27))
    {
        Mat interactionframe;
        interaction >> interactionframe; // get a new frame from camera
        cvtColor(interactionframe, interactionframe, CV_BGR2GRAY);
        fill >> fillframe;
        Mat output = makeFrame(control, interactionframe, fillframe, threshold);
        imshow("Video", output);
    }
    return 0;
}
