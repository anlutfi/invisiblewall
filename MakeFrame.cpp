#include "MakeFrame.h"

cv::Mat makeFrame(cv::Mat control,
                  cv::Mat interaction,
                  cv::Mat fill,
                  unsigned char threshold,
                  int blurkernelsize,
                  int offsetx,
                  int offsety
                 )
{
    //take the absolute difference between the interaction and the 
    //control images and store it in diff
    cv::Mat diff; 
    cv::absdiff(control, interaction, diff);
    
    //initialize an image to store the final result
    cv::Mat result = cv::Mat::zeros( fill.rows, fill.cols, fill.type() );
    
    //initialize a blank mask
    cv::Mat mask = cv::Mat::zeros(control.rows, control.cols, CV_8UC1);
    
    
    //for every pixel in diff...
    for(int i = 0; i < diff.rows; i++)
    {
        for(int j = 0; j < diff.cols; j++)
        {
            cv::Vec3b pixel = diff.at<cv::Vec3b>(i, j);
            
            //...if the absolute value of the pixel exceeds the threshold,
            //set corresponding mask's pixel to white.
            //Otherwise, leave it black.
            if(sqrt(pixel[0] * pixel[0] +
                    pixel[1] * pixel[1] +
                    pixel[2] * pixel[2]
                   ) >= threshold
              )
            {
               mask.at<unsigned char>(i, j) = 255;
            }
        }
    }
    
    //apply median-blur to mask with kernel size of blurkernelsize
    cv::Mat blurredmask;
    cv::medianBlur(mask, blurredmask, blurkernelsize);
    
    //resize mask's resolution to math fill's
    cv::resize( blurredmask, blurredmask, cv::Size(fill.cols, fill.rows) );
    
    //*
    cv::Mat roit;
    cv::Mat finalmask = cv::Mat::zeros(fill.rows,
                                       fill.cols,
                                       blurredmask.type()
                                      );
    if(offsetx >= 0)
    {
        if(offsety >= 0)
        {
            roit = cv::Mat(blurredmask,
                       cv::Range(0, blurredmask.rows - offsety),
                       cv::Range(0, blurredmask.cols - offsetx)
                      );
        
            roit.copyTo(finalmask(cv::Rect(offsetx,
                                           offsety,
                                           roit.cols,
                                           roit.rows
                                          )
                                 )
                       );
        }
        else
        {
            roit = cv::Mat(blurredmask,
                       cv::Range(-offsety, blurredmask.rows),
                       cv::Range(0, blurredmask.cols - offsetx)
                      );
        
            roit.copyTo( finalmask(cv::Rect(offsetx, 0, roit.cols, roit.rows)) );
        }
    }
    else
    {
        if(offsety >= 0)
        {
            roit = cv::Mat(blurredmask,
                       cv::Range(0, blurredmask.rows - offsety),
                       cv::Range(-offsetx, blurredmask.cols)
                      );
            roit.copyTo( finalmask(cv::Rect(0, offsety, roit.cols, roit.rows)) );
        }
        else
        {
            roit = cv::Mat(blurredmask,
                       cv::Range(-offsety, blurredmask.rows),
                       cv::Range(-offsetx, blurredmask.cols)
                      );
            roit.copyTo( finalmask(cv::Rect(0, 0, roit.cols, roit.rows)) );
        }
        
    }
    //*/
    
    //apply mask to fill image and return it
    fill.copyTo(result, finalmask);
    return result;
}
