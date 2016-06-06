#include "LiveMask.h"

int liveMask(cv::Mat control, cv::VideoCapture interaction, cv::VideoCapture fill, int threshold)
{
    Mat fillframe;
    
    if( !interaction.isOpened() || !fill.isOpened() )  // check if we succeeded
        return -1;
    
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    
    Mat interactionframe;
        
    int blurkernelsize;
    
    for(int key = -1; key != 27; key = waitKey(10) % 256)
    {
        interaction >> interactionframe; // get a new frame from camera
        fill >> fillframe;
        Mat output = makeFrameColor(control, interactionframe, fillframe, threshold, blurkernelsize);
        imshow("Video", output);
        
        //TODO REVIEW MAX AND MIN SIZES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111111111111111111111111111
        if( (char)key == '+' )
            threshold += 1;
        else if( (char)key == '-' )
            threshold -= 1;
        else if( (char)key == 'p' )
            blurkernelsize += 2;
        else if( (char)key == 'o' )
            blurkernelsize = max(blurkernelsize - 2, 1);
        else if( (char)key == 'c' )
        {
            control = interactionframe;
        }
    }
    return 0;
}
