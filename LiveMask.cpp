#include "LiveMask.h"

int liveMask(cv::VideoCapture interaction,
             cv::VideoCapture fill,
             cv::Size desiredres,
             void (*reorientInteraction)(cv::Mat),
             void (*reorientFill)(cv::Mat),
             cv::VideoWriter* video
            )
{
    //check if VideoCaptures are working
    if( !interaction.isOpened() || !fill.isOpened() )
        return -1;
    
    //Try to set both fill and interaction cameras' resolutions to desiredres
    fill.set(CAP_PROP_FRAME_WIDTH, desiredres.width);
    fill.set(CAP_PROP_FRAME_HEIGHT, desiredres.height);
    
    std::cout << "\nInteraction camera resolution: "
              << fill.get(CAP_PROP_FRAME_WIDTH)
              << " : "
              << fill.get(CAP_PROP_FRAME_HEIGHT)
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
    Size resolution( fill.get(CAP_PROP_FRAME_WIDTH), fill.get(CAP_PROP_FRAME_HEIGHT) );
    
    //make a black control image with the correct type and resolution
    //Mat control = Mat::zeros( resolution.height, resolution.width, fillframe.type() );
    Mat control = Mat::zeros( interaction.get(CAP_PROP_FRAME_HEIGHT), interaction.get(CAP_PROP_FRAME_WIDTH), fillframe.type() );
    
    //open an output window and set it to full-screen
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);    
    
    //the threshold to be used for pixel election on the mask making process
    unsigned char threshold = DEFAULT_THRESHOLD;    
    
    //the size of the kernel to be used on the mask's median blur
    int blurkernelsize = BL_KERNEL_MIN;
    
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
        Mat output = makeFrame(control, interactionframe, fillframe, threshold, blurkernelsize);
        
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
                threshold = min(threshold + THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << (int)threshold << "\n";
                break;
            
            //decrease threshold
            case '-':
                threshold = max(threshold - THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << (int)threshold << "\n";
                break;
            
            //increase the threshold ten times the usual step
            case ']':
                threshold = min(threshold + 10 * THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << (int)threshold << "\n";
                break;
            
            //decrease the threshold ten times the usual step
            case '[':
                threshold = max(threshold - 10 * THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << (int)threshold << "\n";
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

std::mutex readjust_mtx;

int liveMaskMulti(cv::VideoCapture interaction,
                  cv::VideoCapture fill,
                  cv::Size desiredres,
                  void (*reorientInteraction)(cv::Mat),
                  void (*reorientFill)(cv::Mat),
                  cv::VideoWriter* video
                 )
{
    //check if VideoCaptures are working
    if( !interaction.isOpened() || !fill.isOpened() )
        return -1;
    
    //Try to set both fill and interaction cameras' resolutions to desiredres
    fill.set(CAP_PROP_FRAME_WIDTH, desiredres.width);
    fill.set(CAP_PROP_FRAME_HEIGHT, desiredres.height);
    
    std::cout << "\nInteraction camera resolution: "
              << fill.get(CAP_PROP_FRAME_WIDTH)
              << " : "
              << fill.get(CAP_PROP_FRAME_HEIGHT)
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
    Size resolution( fill.get(CAP_PROP_FRAME_WIDTH), fill.get(CAP_PROP_FRAME_HEIGHT) );
    
    //make a black control image with the correct type and resolution
    //Mat control = Mat::zeros( resolution.height, resolution.width, fillframe.type() );
    Mat control = Mat::zeros( interaction.get(CAP_PROP_FRAME_HEIGHT), interaction.get(CAP_PROP_FRAME_WIDTH), fillframe.type() );
    
    //open an output window and set it to full-screen
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);    
    cv::imshow("Video", fillframe);
    //the threshold to be used for pixel election on the mask making process
    unsigned char threshold = DEFAULT_THRESHOLD;    
    
    //the size of the kernel to be used on the mask's median blur
    int blurkernelsize = BL_KERNEL_MIN;
    
    //*
    std::thread videothd(run,
                         &control,
                         interaction,
                         fill,
                         &threshold,
                         &blurkernelsize,
                         reorientInteraction,
                         reorientFill,
                         video
                        );
    //*/
    
    //keep streaming until user presses esc(27)
    for(unsigned char key = -1; key != 27; key = waitKey(10) % 256)
    {
        //get user input and adjust accordingly
        switch(key)
        {
            //increase threshold for a pixel to be included in the mask
            case '=':
                readjust_mtx.lock();
                threshold = min(threshold + THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << (int)threshold << "\n";
                readjust_mtx.unlock();
                break;
            
            //decrease threshold
            case '-':
                readjust_mtx.lock();
                threshold = max(threshold - THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << (int)threshold << "\n";
                readjust_mtx.unlock();
                break;
            
            //increase the threshold ten times the usual step
            case ']':
                readjust_mtx.lock();
                threshold = min(threshold + 10 * THRESHOLD_STEP, THRESHOLD_MAX);
                std::cout << "Threshold = " << (int)threshold << "\n";
                readjust_mtx.unlock();
                break;
            
            //decrease the threshold ten times the usual step
            case '[':
                readjust_mtx.lock();
                threshold = max(threshold - 10 * THRESHOLD_STEP, THRESHOLD_MIN);
                std::cout << "Threshold = " << (int)threshold << "\n";
                readjust_mtx.unlock();
                break;
            
            //increase kernel size for median blur
            case 'p':
                readjust_mtx.lock();
                blurkernelsize = min(blurkernelsize + BL_KERNEL_STEP, BL_KERNEL_MAX);
                std::cout << "Kernel size = " << blurkernelsize << "\n";
                readjust_mtx.unlock();
                break;
            
            //decerase kernel size
            case 'o':
                readjust_mtx.lock();
                blurkernelsize = max(blurkernelsize - BL_KERNEL_STEP, BL_KERNEL_MIN);
                std::cout << "Kernel size = " << blurkernelsize << "\n";
                readjust_mtx.unlock();
                break;
            
            //generate a new control image(recalibration)
            case 'c':
                readjust_mtx.lock();
                interaction >> control;
                reorientInteraction(control);
                readjust_mtx.unlock();
        }
    }
    
    videothd.join();
    destroyAllWindows();
    
    return 0;
}

int run(cv::Mat* control,
        cv::VideoCapture interaction,
        cv::VideoCapture fill,
        unsigned char* threshold,
        int* blurkernelsize,
        void (*reorientInteraction)(cv::Mat),
        void (*reorientFill)(cv::Mat),
        cv::VideoWriter* video
       )
{
    Mat fillframe, interactionframe;
    
    while(true)
    {
        //*
        readjust_mtx.lock();
        
        //*
        //get a frame from interaction and reorient it
        interaction >> interactionframe; 
        if(reorientInteraction != NULL)
            reorientInteraction(interactionframe);
        
        //get a frame
        fill >> fillframe;
        if(reorientFill != NULL)
            reorientFill(fillframe);
        
        //make a frame using interaction as mask and fill
        Mat output = makeFrame(*control, interactionframe, fillframe, *threshold, *blurkernelsize);
                
        //display frame
        imshow("Video", output);
        
        //if an output video is specified, write the frame into it
        if(video != NULL)
            video->write(output);
        
        readjust_mtx.unlock();
    //*/
    }
}
























