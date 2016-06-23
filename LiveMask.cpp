#include "LiveMask.h"

int liveMask(cv::Mat control, cv::VideoCapture interaction, cv::VideoCapture fill, int threshold)
{
    Mat fillframe;
    
    if( !interaction.isOpened() || !fill.isOpened() )  // check if we succeeded
        return -1;
    
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    
    Mat interactionframe;
        
    int blurkernelsize = 1;
    
    for(int key = -1; key != 27; key = waitKey(10) % 256)
    {
        interaction >> interactionframe; // get a new frame from camera
        fill >> fillframe;
        Mat output = makeFrameColor(control, interactionframe, fillframe, threshold, blurkernelsize);
        imshow("Video", output);
        
        //TODO REVIEW MAX AND MIN SIZES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111111111111111111111111111
        if( (char)key == '=' )
        {
            threshold += 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == '-' )
        {
            threshold -= 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == 'p' )
        {
            blurkernelsize += 2;
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'o' )
        {
            blurkernelsize = max(blurkernelsize - 2, 1);
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'c' )
        {
            control = interactionframe.clone();
        }
    }
    return 0;
}

int liveMask( cv::VideoCapture interaction,
              cv::VideoCapture fill,
              int threshold,
              void (*reorient)(cv::Mat),
              cv::VideoWriter* video
            )
{
    Mat fillframe;
    
    
    if( !interaction.isOpened() || !fill.isOpened() )  // check if we succeeded
        return -1;
    
    //*
    fill.set(CAP_PROP_FRAME_WIDTH,  1920);
    fill.set(CAP_PROP_FRAME_HEIGHT, 1080);
    
    interaction.set(CAP_PROP_FRAME_WIDTH,  1920);
    interaction.set(CAP_PROP_FRAME_HEIGHT, 1080);
    
    fill >> fillframe;
    Size resolution( fill.get(CAP_PROP_FRAME_WIDTH), fill.get(CAP_PROP_FRAME_HEIGHT) );
    Mat control = Mat::zeros( resolution.height, resolution.width, fillframe.type() );
    
    //*/
    cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    
    Mat interactionframe;
    
        
    int blurkernelsize = 1;
    
    for(int key = -1; key != 27; key = waitKey(10) % 256)
    {
        interaction >> interactionframe; // get a new frame from camera
        if(reorient != NULL)
            reorient(interactionframe);
        
        //*
        cv::resize(interactionframe, interactionframe, resolution);
        
          
            
        fill >> fillframe;
        Mat output = makeFrameColor(control, interactionframe, fillframe, threshold, blurkernelsize);
        imshow("Video", output);
        
        if(video != NULL)
            video->write(output);
        
        //TODO REVIEW MAX AND MIN SIZES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111111111111111111111111111
        if( (char)key == '=' )
        {
            threshold += 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == '-' )
        {
            threshold -= 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == 'p' )
        {
            blurkernelsize += 2;
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'o' )
        {
            blurkernelsize = max(blurkernelsize - 2, 1);
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'c' )
        {
            control = interactionframe.clone();
        }
    }
    return 0;
}


int liveMask(cv::Mat control, VideoFeed interaction, VideoFeed fill, int threshold)
{
    Mat fillframe;
    
    std::cout << "\n\nfill: " << fill.capture->get(CAP_PROP_FRAME_WIDTH) << " " << fill.capture->get(CAP_PROP_FRAME_HEIGHT) << "\n";
    std::cout << "interaction: " << interaction.capture->get(CAP_PROP_FRAME_WIDTH) << " " << interaction.capture->get(CAP_PROP_FRAME_HEIGHT) << "\n";
    
    cv::namedWindow("Video", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Video", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    
    //*
    std::thread interactionthrd(&VideoFeed::run, &interaction);
    std::thread fillthrd(&VideoFeed::run, &fill);
    //*/
    Mat interactionframe;
        
    int blurkernelsize = 1;
    
    for(int key = -1; key != 27; key = waitKey(10) % 256)
    {
        interactionframe = interaction.getFrame();
        fillframe = fill.getFrame();
        
        Mat output = makeFrameColor(control, interactionframe, fillframe, threshold, blurkernelsize);
        imshow("Video", output);
        
        //TODO REVIEW MAX AND MIN SIZES!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1111111111111111111111111111
        if( (char)key == '=' )
        {
            threshold += 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == '-' )
        {
            threshold -= 1;
            std::cout << "Threshold = " << threshold << "\n";
        }
        else if( (char)key == 'p' )
        {
            blurkernelsize += 2;
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'o' )
        {
            blurkernelsize = max(blurkernelsize - 2, 1);
            std::cout << "Kernel size = " << blurkernelsize << "\n";
        }
        else if( (char)key == 'c' )
        {
            control = interactionframe.clone();
        }
    }
    
    //*
    interactionthrd.join();
    fillthrd.join();
    //*/
    
    return 0;
}
