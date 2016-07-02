#include "Calibration.h"

void assignCameras(VideoCapture* interaction, VideoCapture* fill, int i1, int i2)
{
    VideoCapture* caps[MAX_CAM];
    caps[0] = new VideoCapture();
    caps[1] = new VideoCapture();
    
    try
    {
        caps[0]->open(i1);
        caps[1]->open(i2);
    }
    catch(...)
    {
    }
    
    for(unsigned char i = 0; i < MAX_CAM; i++)
        if( !caps[i]->isOpened() )
        {
            delete caps[i];
            continue;
        }
    
    namedWindow("Camera Assignment", CV_WINDOW_AUTOSIZE);
    
    for(unsigned char i = 0; i < MAX_CAM; i++)
    {
        VideoCapture* cap = caps[i];
        char key;
        for(key = ' ';
            tolower(key) != 'i' && tolower(key) != 'e' && tolower(key) != 'n';
            key = waitKey(10)
           )
        {
            Mat frame;
            (*cap) >> frame;
            imshow("Camera Assignment", frame);
        }
        
        switch(key)
        {
            case 'i':
                *interaction = *cap;
                break;
            
            case 'e':
                *fill = *cap;
                break;
            
            case 'n':
                delete cap;
        }
    }
    destroyWindow("Camera Assignment");
    destroyAllWindows();
    waitKey(1);
}

void calibrate(VideoCapture interaction,
               VideoCapture fill,
               unsigned char* threshold,
               int* blurkernelsize,
               int* offsetx,
               int* offsety,
               int offsetstep,
               Size desiredres
              )
{
    fill.set(CAP_PROP_FRAME_WIDTH, desiredres.width);
    fill.set(CAP_PROP_FRAME_HEIGHT, desiredres.height);
    
    Size resolution(fill.get(CAP_PROP_FRAME_WIDTH),
                    fill.get(CAP_PROP_FRAME_HEIGHT)
                   );
    
    namedWindow("Calibration", CV_WINDOW_NORMAL);
    
    Mat control, fillframe, interactionframe, frame;
    
    cout << "Take a control image, press SPACEBAR until the image is good and then ENTER\n";
    for(unsigned char key = 0; key != 10/*ENTER*/; key = waitKey(0))
    {  
        interaction >> control;
        
        imshow("Calibration", control);
    }
    
    cout << "Position objects or people in front of the internal camera. "
         << "Press SPACEBAR until image is good and then press ENTER.\n";
    for(unsigned char key = 0; key != 10/*ENTER*/; key = waitKey(0))
    {
        interaction >> interactionframe;
        
        imshow("Calibration", interactionframe);
    }
    
    cout << "Position objects or people in front of the external camera. "
         << "Use color differences and a lot of light for best calibration results. "
         << "Press SPACEBAR until image is good and then press ENTER.\n";
    for(unsigned char key = 0; key != 10/*ENTER*/; key = waitKey(0))
    {
        fill >> fillframe;
        
        imshow("Calibration", fillframe);
    }
    
    
    cout << "Adjust Threshold, press LEFT or RIGHT until image is acceptable and then ENTER\n"
         << "Use UP or DOWN to increase and decrease threshold faster\n";
    for(unsigned char key = 0; key != 10/*ENTER*/; key = waitKey(0))
    {
        frame = makeFrame(control,
                          interactionframe,
                          fillframe,
                          *threshold,
                          *blurkernelsize,
                          *offsetx,
                          *offsety
                         );
                             
        imshow("Calibration", frame);
        
        switch(key)
        {
            //increase threshold for a pixel to be included in the mask
            case RIGHT_KEY:
                *threshold = min(*threshold + THRESHOLD_STEP, THRESHOLD_MAX);
                break;
            
            //decrease threshold
            case LEFT_KEY:
                *threshold = max(*threshold - THRESHOLD_STEP, THRESHOLD_MIN);
                break;
            
            //increase the threshold ten times the usual step
            case UP_KEY:
                *threshold = min(*threshold + 10 * THRESHOLD_STEP, THRESHOLD_MAX);
                break;
            
            //decrease the threshold ten times the usual step
            case DOWN_KEY:
                *threshold = max(*threshold - 10 * THRESHOLD_STEP, THRESHOLD_MIN);
                break;
        }
    }
    
    cout << "Adjust Blur, press LEFT or RIGHT until image is acceptable and then ENTER\n";
    for(unsigned char key = 0; key != 10/*ENTER*/; key = waitKey(0))
    {
        frame = makeFrame(control,
                          interactionframe,
                          fillframe,
                          *threshold,
                          *blurkernelsize,
                          *offsetx,
                          *offsety
                         );
                             
        imshow("Calibration", frame);
        
        switch(key)
        {
            //increase kernel size for median blur
            case RIGHT_KEY:
                *blurkernelsize = min(*blurkernelsize + BL_KERNEL_STEP,
                                      BL_KERNEL_MAX
                                     );
                break;
            
            //decerase kernel size
            case LEFT_KEY:
                *blurkernelsize = max(*blurkernelsize - BL_KERNEL_STEP,
                                      BL_KERNEL_MIN
                                     );
                break;
        }
    }
    
    cout << "Adjust Offset, press LEFT, RIGHT, UP or DOWN until silhouette"
         << " is at the correct position and then press ENTER\n"
         << "Pressing 0 will re-center\n";
    for(unsigned char key = 0; key != 10/*ENTER*/; key = waitKey(0))
    {
        frame = makeFrame(control,
                          interactionframe,
                          fillframe,
                          *threshold,
                          *blurkernelsize,
                          *offsetx,
                          *offsety
                         );
                             
        imshow("Calibration", frame);
        
        switch(key)
        {
            case LEFT_KEY:
                *offsetx = abs(*offsetx - offsetstep) <= resolution.width ?
                           *offsetx - offsetstep : -resolution.width + 1;
                break;
                
            case RIGHT_KEY:
                *offsetx = abs(*offsetx + offsetstep) <= resolution.width ?
                           *offsetx + offsetstep : resolution.width - 1;
                break;
                
            case UP_KEY:
                *offsety = abs(*offsety - offsetstep) <= resolution.height ?
                           *offsety - offsetstep : -resolution.height + 1;
                break;
                
            case DOWN_KEY:
                *offsety = abs(*offsety + offsetstep) <= resolution.height ?
                           *offsety + offsetstep : resolution.height - 1;
                break;
            
            case '0':
                *offsetx = *offsety = 0;
                break;
        }
    }
}













