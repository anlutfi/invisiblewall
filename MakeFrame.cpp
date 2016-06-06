#include "MakeFrame.h"

cv::Mat makeFrame(cv::Mat control, cv::Mat interaction, cv::Mat fill, int threshold)
{
    cv::Mat diff; 
    cv::absdiff(control, interaction, diff);
    
    cv::namedWindow("diff", CV_WINDOW_NORMAL);
    imshow("diff", diff);
    cv::waitKey(27);

    
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

cv::Mat makeFrameColor(cv::Mat control, cv::Mat interaction, cv::Mat fill, int threshold)
{
    cv::Mat diff; 
    cv::absdiff(control, interaction, diff);
    
    cv::namedWindow("diff", CV_WINDOW_NORMAL);
    imshow("diff", diff);
    cv::waitKey(27);

    
    cv::Mat result = cv::Mat::zeros(control.rows, control.cols, fill.type());
    
    for(int i = 0; i < diff.rows; i++)
    {
        for(int j = 0; j < diff.cols; j++)
        {
            cv::Vec3b pixel = diff.at<cv::Vec3b>(i, j);
            
            if( sqrt(pixel[0] * pixel[0] + pixel[1] * pixel[1] + pixel[2] * pixel[2]) >= threshold )
            {
               result.at<cv::Vec3b>(i, j) = fill.at<cv::Vec3b>(i, j);
            }
        }
    }
    
    return result;
}
