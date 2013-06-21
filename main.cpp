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

int main(int argc,char** argv)
{

//	VideoCapture cap;
	int f=0;
	
//	cap.open(argv[1]);
//	Mat image;
//	cap >> image;

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
//	src_image.copyTo(dst);
	Mat warp = Mat::zeros(image.height,image.width,CV_8UC3);
//	Mat src_image,warp;
	
	Mat warp_mat;
	Point2f* pts;
	pts = new Point2f [4];
	
	ball oball = ball();
	arena_center = Point(image.width/2,image.height/2);

	int k = 0;

	char c = 0;
	our_bot bot;
	
	bot.basecolor = 'y';
	bot.lcolor = 'v';
	bot.rcolor = 'g';

	double time_for_warp = 0;
	double time_for_init = 0;
	double time_for_resize = 0;
	double time_for_bot = 0;
	double time_for_imshow = 0;
	double time_for_loop = 0;
	double time_for_cap = 0;
	double time_for_cvt = 0;
	double time_for_rect = 0;
	while(1)
	{	

		
		time_for_loop = (double)cvGetTickCount();

		time_for_cap = (double)cvGetTickCount();

		// getting image from camera
		stat = xiGetImage(xiH, 1000, &image);
		HandleResult(stat,"xiGetImage");
		src_image.data = (uchar*)image.bp;

//		cap >> src_image;
		time_for_cap = ((double)cvGetTickCount() - time_for_cap)/(1000.0*(double)cvGetTickFrequency());
		
		time_for_resize = (double)cvGetTickCount();
		resize(src_image,dst,Size(640,480));
		time_for_resize = ((double)cvGetTickCount() - time_for_resize)/(1000.0*(double)cvGetTickFrequency());
		
		if(f==1)
		{
//			time_for_warp = (double)cvGetTickCount();
//			warp = perspectiveArena(warp_mat,dst);
//			warp = dst(Rect(pts[0],pts[2]));
//			warp = dst;
			dst.copyTo(warp);
//			time_for_warp = ((double)cvGetTickCount() - time_for_warp)/(1000.0*(double)cvGetTickFrequency());
			
			time_for_init= (double)cvGetTickCount();
			oball.init(warp);
			time_for_init = ((double)cvGetTickCount() - time_for_init)/(1000.0*(double)cvGetTickFrequency());
		}
		else if(f==0)
		{
//			pts = selectPoint(dst);
			pts[0].x = 49;
			pts[0].y = 33;
			pts[1].x = 568;
			pts[1].y = 35;
			pts[2].x = 564;
			pts[2].y = 431;
			pts[3].x = 55;
			pts[3].y = 435;
			warp_mat=getTransformMat(pts);

			f=1;
			destroyAllWindows();
		}

//		warp.convertTo(warp,CV_8UC3,255);

		time_for_cvt = (double)cvGetTickCount();
		cvtColor(warp, dst, CV_BGR2HSV);
		time_for_cvt = ((double)cvGetTickCount() - time_for_cvt)/(1000.0*(double)cvGetTickFrequency());

//		time_for_bot = (double)cvGetTickCount();
		bot.update();
//		time_for_bot = ((double)cvGetTickCount() - time_for_bot)/(1000.0*(double)cvGetTickFrequency());
		
//		time_for_rect= (double)cvGetTickCount();
		rectangle(warp,Point(bot.bot_center.x-15,bot.bot_center.y-15),Point(bot.bot_center.x+15,bot.bot_center.y+15), Scalar(0,0,255));
//		time_for_rect = ((double)cvGetTickCount() - time_for_rect)/(1000.0*(double)cvGetTickFrequency());
		rectangle(warp,oball.bounding_box,Scalar(0,0,255));
		
//		time_for_imshow = (double)cvGetTickCount();
		imshow("warp",warp);
//		time_for_imshow = ((double)cvGetTickCount() - time_for_imshow)/(1000.0*(double)cvGetTickFrequency());
		time_for_loop = ((double)cvGetTickCount() - time_for_loop)/(1000.0*(double)cvGetTickFrequency());
		
//		printf("\nTime for cap : %lf\n",(double)time_for_cap);
//		printf("\nTime for resize : %lf\n",(double)time_for_resize);
//		printf("\nTime for warp : %lf\n",(double)time_for_warp);
//		printf("\nTime for init : %lf\n",(double)time_for_init);
//		printf("\nTime for bot : %lf\n",(double)time_for_bot);
//		printf("\nTime for imshow : %lf\n",(double)time_for_imshow);
//		printf("\nTime for cvt : %lf\n",(double)time_for_cvt);
//		printf("\nTime for rect : %lf\n",(double)time_for_rect);
		printf("\nTime for loop : %lf\n",(double)time_for_loop);

		c = waitKey(1);
		if(c==27)
			break;
		if(c == ' ')
			c=waitKey(0);

	}
	delete pts;
	destroyAllWindows();
//	cap.release();
}	

