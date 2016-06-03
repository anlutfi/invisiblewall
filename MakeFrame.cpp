#include "MakeFrame.h"

cv::Mat makeFrame(cv::Mat control, cv::Mat interaction, cv::Mat fill, int threshold)
{
    cv::Mat diff; 
    cv::absdiff(control, interaction, diff);
    
    cv::Mat result = cv::Mat::zeros(control.rows, control.cols, fill.type());
    
    for(int i = 0; i < diff.rows; i++)
    {
        for(int j = 0; j < diff.cols; j++)
        {
            if(diff.at<uchar>(i, j) >= threshold)
            {
               result.at<cv::Vec3b>(i, j) = fill.at<cv::Vec3b>(i, j);
            }
        }
    }
    
    return result;
}

