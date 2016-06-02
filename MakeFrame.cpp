#include "MakeFrame.h"

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
