#include "arena.h"
#include "ball.h"
#include "our_bot.h"
#include "opp_bot.h"
#include <xiApi.h>
#include <xiExt.h>
#include <iostream>

#define HandleResult(res,place) if (res!=XI_OK) { \
    printf("Error after %s (%d)",place,res); \
    if (xiH) xiCloseDevice(xiH); \
}

using namespace cv;
using namespace std;

Mat dst;

Point arena_center;

main(int argc,char** argv)
{

    VideoCapture cap;
    int f=0;

    HANDLE xiH = NULL;
    XI_RETURN stat = XI_OK;

    // image buffer
    XI_IMG image;
    image.size = sizeof(XI_IMG);
    image.bp = NULL;
    image.bp_size = 0;

    // Retrieving a handle to the camera device 
    stat = xiOpenDevice(0, &xiH);
    HandleResult(stat,"xiOpenDevice");

    // Setting "exposure" parameter (10ms)
    int time_us = 10000;
    stat = xiSetParam(xiH, XI_PRM_EXPOSURE, &time_us, sizeof(time_us), xiTypeInteger);
    HandleResult(stat,"xiSetParam (exposure set)");

    int binning_mode = 2;
    // Change any of the camera's parameter here
    xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24);
    xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING, binning_mode);
    xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING_TYPE, 1);

    xiSetParamInt(xiH, XI_PRM_RECENT_FRAME, 0);
    xiSetParamInt(xiH, XI_PRM_BUFFERS_QUEUE_SIZE, 3);
    xiSetParamInt(xiH, XI_PRM_ACQ_BUFFER_SIZE, 20*1024*1024);
    
    // Start acquisition
    stat = xiStartAcquisition(xiH);
    HandleResult(stat,"xiStartAcquisition");

    stat = xiGetImage(xiH, 1000, &image);
    HandleResult(stat,"xiGetImage");
    
    Mat src_image = Mat::zeros(image.height,image.width,CV_8UC3);
    dst = Mat::zeros(image.height,image.width,CV_8UC3);
    src_image.copyTo(dst);
    Mat warp = Mat::zeros(image.height,image.width,CV_8UC3);
    Mat warp_mat;
    Point2f* pts;
    pts = new Point2f [4];
    ball oball = ball();
    arena_center = Point(image.width/2,image.height/2);

    int k = 0;

    char c = 0;
    our_bot bot;

    double time_for_warp = 0;
    double time_for_init = 0;
    double time_for_resize = 0;	
    while(1)
    {	
    
        // getting image from camera
    stat = xiGetImage(xiH, 1000, &image);
    HandleResult(stat,"xiGetImage");
    
    src_image.data = (uchar*)image.bp;
        time_for_resize = (double)cvGetTickCount();
        resize(src_image,dst,Size(640,480));
        time_for_resize = ((double)cvGetTickCount() - time_for_resize)/(1000.0*(double)cvGetTickFrequency());


        if(f==1)
        {
            time_for_warp = (double)cvGetTickCount();
            warp = perspectiveArena(warp_mat,dst);
            time_for_warp = ((double)cvGetTickCount() - time_for_warp)/(1000.0*(double)cvGetTickFrequency());
            time_for_init= (double)cvGetTickCount();
            oball.init(warp);
            time_for_init = ((double)cvGetTickCount() - time_for_init)/(1000.0*(double)cvGetTickFrequency());
        }
        else if(f==0)
        {
            pts = selectPoint(dst);	
            warp_mat=getTransformMat(pts);	

            f=1;
            destroyAllWindows();
        }
        //warp.convertTo(warp,CV_8UC3,255);
        //	printf("\nTime for resize : %lf\n",(double)time_for_resize);
        //    printf("\nTime for warp : %lf\n",(double)time_for_warp);
        //    printf("\nTime for init : %lf\n",(double)time_for_init);

        cvtColor(warp, dst, CV_BGR2HSV);

        bot.basecolor = 'g';
        bot.lcolor = 'b';
        bot.rcolor = 'v';

        //bot.update();

        //rectangle(warp,Point(bot.bot_center.x-15,bot.bot_center.y-15),
         //       Point(bot.bot_center.x+15,bot.bot_center.y+15), Scalar(0,0,255));
        rectangle(warp,oball.bounding_box,Scalar(0,0,255));
        
        

        imshow("warp",warp);

        c = waitKey(20);
        if(c==27)
            break;
        if(c == ' ')
            c=waitKey(0);

    }
    delete pts;
    destroyAllWindows();
    cap.release();
}	

