#include "LiveMask.h"

int liveMask( cv::VideoCapture interaction,
              cv::VideoCapture fill,
              cv::Size desiredres,
              int threshold,
              void (*reorient)(cv::Mat),
              cv::VideoWriter* video
            )
{
    //check if VideoCaptures are working
    if( !interaction.isOpened() || !fill.isOpened() )
        return -1;
    
    //Try to set both fill and interaction cameras' resolutions to desiredres
    fill.set(CAP_PROP_FRAME_WIDTH, desiredres.width);
    fill.set(CAP_PROP_FRAME_HEIGHT, desiredres.height);
    
    interaction.set(CAP_PROP_FRAME_WIDTH, desiredres.width);
    interaction.set(CAP_PROP_FRAME_HEIGHT, desiredres.height);
    
    //get a frame from fill camera
    //because we need its type for the control image
    Mat fillframe, interactionframe;
    fill >> fillframe;
    
    //get the actual resolution from the fill camera
    //after trying to set it to desiredres
    Size resolution( fill.get(CAP_PROP_FRAME_WIDTH), fill.get(CAP_PROP_FRAME_HEIGHT) );
    
    //make a black control image with the correct type and resolution
    Mat control = Mat::zeros( resolution.height, resolution.width, fillframe.type() );
    
    //open an output window and set it to full-screen
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);    
        
    //the size of the kernel to be used on the mask's median blur
    int blurkernelsize = BL_KERNEL_MIN;
    
    //keep streaming until user presses esc(27)
    for(int key = -1; key != 27; key = waitKey(10) % 256)
    {
        //get a frame from interaction and reorient it
        interaction >> interactionframe; 
        if(reorient != NULL)
            reorient(interactionframe);
        
        //scale it to the same size as fill's
        cv::resize(interactionframe, interactionframe, resolution);
        
        //get a frame
        fill >> fillframe;
        
        //make a frame using interaction as mask and fill
        Mat output = makeFrame(control, interactionframe, fillframe, threshold, blurkernelsize);
        
        //display frame
        imshow("Video", output);
        
        //if an output video is specified, write the frame into it
        if(video != NULL)
            video->write(output);
        
        //get user input and adjust accordingly
        switch( (char)key )
        {
            //increase threshold for a pixel to be included in the mask
            case '=':
                threshold = min(threshold + THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << threshold << "\n";
                break;
            
            //decrease threshold
            case '-':
                threshold = max(threshold - THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << threshold << "\n";
                break;
            
            //increase the threshold ten times the usual step
            case ']':
                threshold = min(threshold + 10 * THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << threshold << "\n";
                break;
            
            //decrease the threshold ten times the usual step
            case '[':
                threshold = max(threshold - 10 * THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << threshold << "\n";
                break;
            
            //increase kernel size for median blur
            case 'p':
                blurkernelsize = min(blurkernelsize + BL_KERNEL_STEP, BL_KERNEL_MAX);
                std::cout << "Kernel size = " << blurkernelsize << "\n";
                break;
            
            //decerase kernel size
            case 'o':
                blurkernelsize = max(blurkernelsize - BL_KERNEL_STEP, BL_KERNEL_MIN);
                std::cout << "Kernel size = " << blurkernelsize << "\n";
                break;
            
            //generate a new control image(recalibration)
            case 'c':
                control = interactionframe.clone();
        }
        
    }
    return 0;
}
