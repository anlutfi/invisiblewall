#include "LiveMask.h"

int liveMask(cv::VideoCapture interaction,
             cv::VideoCapture fill,
             cv::Size desiredres,
             void (*reorientInteraction)(cv::Mat),
             void (*reorientFill)(cv::Mat),
             unsigned char* threshold,
             int* blurkernelsize,
             int offsetstep,
             int* maskoffsetx,
             int* maskoffsety,
             cv::VideoWriter* video,
             cv::Size camguardsize
            )
{
    //check if VideoCaptures are working
    if( !interaction.isOpened() || !fill.isOpened() )
        return -1;
    
    //Try to set fill camera's resolution to desiredres
    fill.set(CAP_PROP_FRAME_WIDTH, desiredres.width);
    fill.set(CAP_PROP_FRAME_HEIGHT, desiredres.height);
    
    std::cout << "\nInteraction camera resolution: "
              << interaction.get(CAP_PROP_FRAME_WIDTH)
              << " : "
              << interaction.get(CAP_PROP_FRAME_HEIGHT)
              << "\n";
    
    std::cout << "Fill camera resolution: "
              << fill.get(CAP_PROP_FRAME_WIDTH)
              << " : "
              << fill.get(CAP_PROP_FRAME_HEIGHT)
              << "\n";
    
    
    //get a frame from fill camera
    //because we need its type for the control image
    Mat fillframe, interactionframe;
    fill >> fillframe;
    
    //get the actual resolution from the fill camera
    //after trying to set it to desiredres
    Size resolution(fill.get(CAP_PROP_FRAME_WIDTH),
                    fill.get(CAP_PROP_FRAME_HEIGHT)
                   );
    
    //make a black control image with the correct type and resolution
    Mat control;
    interaction >> control;
    if(reorientInteraction != NULL)
            reorientInteraction(control);
    
    //open an output window and set it to full-screen
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);    
    
    
    //keep streaming until user presses esc(27)
    for(unsigned char key = -1; key != 27; key = waitKey(10) % 256)
    {
        //get a frame from interaction and reorient it
        interaction >> interactionframe; 
        if(reorientInteraction != NULL)
            reorientInteraction(interactionframe);
        
        //get a frame
        fill >> fillframe;
        if(reorientFill != NULL)
            reorientFill(fillframe);
        
        
        //make a frame using interaction as mask and fill
        Mat output = makeFrame(control,
                               interactionframe,
                               fillframe,
                               *threshold,
                               *blurkernelsize,
                               *maskoffsetx,
                               *maskoffsety/*,
                               camguardpos,
                               camguardsize*/
                              );
        
        //display frame
        imshow("Video", output);
        
        //if an output video is specified, write the frame into it
        if(video != NULL)
            video->write(output);
        
        //get user input and adjust accordingly
        switch(key)
        {
            //increase threshold for a pixel to be included in the mask
            case '=':
                *threshold = min(*threshold + THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << (int)(*threshold) << "\n";
                break;
            
            //decrease threshold
            case '-':
                *threshold = max(*threshold - THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << (int)(*threshold) << "\n";
                break;
            
            //increase the threshold ten times the usual step
            case ']':
                *threshold = min(*threshold + 10 * THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << (int)(*threshold) << "\n";
                break;
            
            //decrease the threshold ten times the usual step
            case '[':
                *threshold = max(*threshold - 10 * THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << (int)(*threshold) << "\n";
                break;
            
            case 'b':
                *blurkernelsize = *blurkernelsize == 1 ? 3 : 1;
                break;
            
            //generate a new control image(recalibration)
            case 'c':
                control = interactionframe.clone();
                break;
                
            //move the mask around, so the silhouette's are exactly in front of the subjects
            case UP_KEY:
                *maskoffsety = abs(*maskoffsety - offsetstep) <= resolution.height ?
                              *maskoffsety - offsetstep : -resolution.height + 1;
                std::cout << "Y offset = " << *maskoffsety << "\n";
                break;
            
            case DOWN_KEY:
                *maskoffsety = abs(*maskoffsety + offsetstep) <= resolution.height ?
                              *maskoffsety + offsetstep : resolution.height - 1;
                std::cout << "Y offset = " << *maskoffsety << "\n";
                break;
                
            case LEFT_KEY:
                *maskoffsetx = abs(*maskoffsetx - offsetstep) <= resolution.width ?
                              *maskoffsetx - offsetstep : -resolution.width + 1;
                std::cout << "X offset = " << *maskoffsetx << "\n";
                break;
            
            case RIGHT_KEY:
                *maskoffsetx = abs(*maskoffsetx + offsetstep) <= resolution.width ?
                              *maskoffsetx + offsetstep : resolution.width - 1;
                std::cout << "X offset = " << *maskoffsetx << "\n";
                break;
            
            //reset offset
            case '0':
                *maskoffsetx = *maskoffsety = 0;
                std::cout << "X offset = " << *maskoffsetx << "\n";
                std::cout << "Y offset = " << *maskoffsety << "\n";
                break;
        }
        
    }
    return 0;
}
