#ifndef __MAKE_FRAME_H__
#define __MAKE_FRAME_H__

//#include <opencv/cv.h>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/core/utility.hpp>
//#include <iostream>
//#include <time.h>

/*
Function that makes a black frame with the interactors silhouettes
filled with a filling image.

control is a gray-scale control image, a picture of the empty interaction space

interaction is a gray-scale picture of the filled interaction space

fill is a picture to fill the silhoutes in the resulting image, tipically,
the other side of the projection wall.

threshold is a number between 0 and 255. Corresponding pixels in control and
interaction will be used as masks if the absolute difference between them is
larger or equal to threshold.

blurkernelsize is the size of the kernel to be used on the median-blur,
to reduce noise on the mask. 
*/
cv::Mat makeFrame(cv::Mat control,
                  cv::Mat interaction,
                  cv::Mat fill,
                  unsigned char threshold,
                  int blurkernelsize,
                  int offsetx,
                  int offsety
                 );

#endif
