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

cv::Mat makeFrameColor(cv::Mat control, cv::Mat interaction, cv::Mat fill, int threshold, int blurkernelsize)
{
    cv::Mat diff; 
    cv::absdiff(control, interaction, diff);
    
    cv::Mat blurreddiff;
    cv::medianBlur( diff, blurreddiff, blurkernelsize );
    
    cv::Mat result = cv::Mat::zeros(control.rows, control.cols, fill.type());
    cv::Mat mask = cv::Mat::zeros(control.rows, control.cols, CV_8UC1);
    
    
    for(int i = 0; i < blurreddiff.rows; i++)
    {
        for(int j = 0; j < blurreddiff.cols; j++)
        {
            cv::Vec3b pixel = blurreddiff.at<cv::Vec3b>(i, j);
            
            if( sqrt(pixel[0] * pixel[0] + pixel[1] * pixel[1] + pixel[2] * pixel[2]) >= threshold )
            {
               mask.at<unsigned char>(i, j) = 255;
            }
        }
    }
    fill.copyTo(result, mask);
    
    return result;
}

cv::Mat makeFrameColor( cv::Mat control,
                        cv::Mat interaction,
                        cv::Mat fill,
                        int threshold,
                        int blurkernelsize,
                        void (*reorient)(cv::Mat)
                      )
{
    cv::Mat diff; 
    cv::absdiff(control, interaction, diff);
    
    cv::Mat blurreddiff;
    cv::medianBlur( diff, blurreddiff, blurkernelsize );
    
    cv::Mat result = cv::Mat::zeros(control.rows, control.cols, fill.type());
    cv::Mat mask = cv::Mat::zeros(control.rows, control.cols, CV_8UC1);
    
    
    for(int i = 0; i < blurreddiff.rows; i++)
    {
        for(int j = 0; j < blurreddiff.cols; j++)
        {
            cv::Vec3b pixel = blurreddiff.at<cv::Vec3b>(i, j);
            
            if( sqrt(pixel[0] * pixel[0] + pixel[1] * pixel[1] + pixel[2] * pixel[2]) >= threshold )
            {
               mask.at<unsigned char>(i, j) = 255;
            }
        }
    }
    fill.copyTo(result, mask);
    
    reorient(result);
    
    return result;
}
