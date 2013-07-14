#include "arena.h"
#include "ball.h"
#include "our_bot.h"
#include "opp_bot.h"
#include <xiApi.h>
#include <xiExt.h>
#include <iostream>
#include "wifi.h"
#include "motion.h"
#include "xbee.h"

#define HandleResult(res,place) if (res!=XI_OK) { \
	printf("Error after %s (%d)",place,res); \
	if (xiH) xiCloseDevice(xiH); \
}

using namespace cv;
using namespace std;

Mat dst;

Point arena_center;

float integral_error[5] = {0,0,0,0,0};  	//For PID
float prev_error[5] = {0,0,0,0,0}; 
int flag_movt[2] = {0,0};

float velocity = 90;

//A small class to make life easier for the ones working with videos
class Image {
    public :
        int width;
        int height;
};

int main(int argc,char** argv)
{

	Point destinations[10];
	int no_of_dest = 8;
	destinations[0] = Point(110,330);
	destinations[1] = Point(140,280);
	destinations[2] = Point(185,265);
	destinations[3] = Point(230,280);
	destinations[4] = Point(260,330);
	destinations[5] = Point(230,370);	
	destinations[6] = Point(185,385);
	destinations[7] = Point(140,370);	
	int count_of_dest = 0;
    float tol=20;


//	VideoCapture cap;
	int f=0;
	
//	cap.open(argv[1]);
//    Image image;
//    image.width = cap.get(3);
//    image.height = cap.get(4);

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
	
	bot.basecolor = 'g';
	bot.lcolor = 'v';
	bot.rcolor = 'g';
	int turn_flag=0;

	double time_for_warp = 0;
	double time_for_init = 0;
	double time_for_resize = 0;
	double time_for_bot = 0;
	double time_for_imshow = 0;
	double time_for_loop = 0;
	double time_for_cap = 0;
	double time_for_cvt = 0;
	double time_for_rect = 0;
	int angle_buffer=0;
	int socket;
//	int socket = init_wifi(9750,"122.38.0.151");
	char command[4];
	int stop=0;
	int velocity=60;
			
	//tcpWrite(socket,command);


//	VideoWriter out("ipfeedback.avi",CV_FOURCC('M','J','P','G'),40,Size(640,480));

//	int socket = init_wifi(9750,"122.38.0.151");
//	char command[4]={'0','F','0','0'};
//	tcpWrite(socket,command);


//	VideoWriter out("ipfeedback.avi",CV_FOURCC('M','J','P','G'),40,Size(640,480));

	while( c != 27 )
	{
		time_for_loop = (double)cvGetTickCount();

		time_for_cap = (double)cvGetTickCount();

		// getting image from camera
		stat = xiGetImage(xiH, 1000, &image);
		HandleResult(stat,"xiGetImage");
		src_image.data = (uchar*)image.bp;

	//	cap >> src_image;
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
			UART_init();
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


		if(count_of_dest == no_of_dest)
			count_of_dest = 0;
		bot.dst_point = destinations[count_of_dest];
//Aby
//		bot.dst_point = destinations[0];

		time_for_cvt = (double)cvGetTickCount();
		cvtColor(warp, dst, CV_BGR2HSV);
		time_for_cvt = ((double)cvGetTickCount() - time_for_cvt)/(1000.0*(double)cvGetTickFrequency());

//		time_for_bot = (double)cvGetTickCount();
		bot.update();
//		time_for_bot = ((double)cvGetTickCount() - time_for_bot)/(1000.0*(double)cvGetTickFrequency());
		bot.bot_angle -= 270; //TODO:Ask Hari to fix 90 deg offset
		cout<<"Distance :"<<bot.distance<<'\t';
		cout<<"Angle :"<<bot.bot_angle<<'\t';
//		if(bot.bot_angle>0)
//		{
			if(bot.bot_angle>180){
				bot.bot_angle = bot.bot_angle-360;
			}
			angle_buffer=bot.bot_angle;
			cout<<"count : "<<count_of_dest<<"\t";
//			movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,2,tol);
//			send_command(command);
//			if(bot.distance>40){
//			if((bot.bot_angle>10||bot.bot_angle<-10) && turn_flag==0)
//			{
//			turn(0,socket,bot.bot_angle,0,integral_error);
//			cout<<"turn"<<endl;
//			}
			if((bot.distance>tol)) 
			{//tol is the tolerance :P
				movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,0,tol);
////				stop=count_of_dest;
////				movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,stop,tol);
////			}
////            else if(stop==2)
////            {
////                // Stopping the motors
////                tol=0;             // TODO Incorporate tol in the movement code
////				movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,stop,tol);
			}
			
			else
			{
//				command[0] = (char)(48+bot_id);
//				command[1] = 's';
//       			command[2] = '0';
//        		command[3] = 's';
//        		send_command(command);
				integral_error[0] = 0;
////				integral_error[1] = 0;
////				integral_error[2] = 0;
////				turn_flag=0;
////				integral_error[3] = 0;
////				integral_error[4] = 0;
				if(count_of_dest<no_of_dest)
					count_of_dest++;
			}
////Aby


//			else
//				while(1);
//			turn(0,socket, angle_buffer,90,integral_error);

//			cout<<(bot.bot_angle)<<'\t';
//		}


		
//		time_for_rect= (double)cvGetTickCount();
		rectangle(warp,Point(bot.bot_center.x-15,bot.bot_center.y-15),Point(bot.bot_center.x+15,bot.bot_center.y+15), Scalar(0,0,255));
		for(int i =0; i<no_of_dest;i++){
			circle(warp,Point(destinations[i].x,destinations[i].y),3,Scalar(255,255,255),-1);
			circle(warp,Point(destinations[i].x,destinations[i].y),tol,Scalar(255,255,255),1);}
//		circle(warp,Point(destinations[0].x,destinations[0].y),45,Scalar(255,255,255),1);
//		circle(warp,Point(destinations[1].x,destinations[1].y),3,Scalar(255,255,255),-1);
//		circle(warp,Point(destinations[1].x,destinations[1].y),15,Scalar(255,255,255),1);
//		circle(warp,Point(destinations[2].x,destinations[2].y),3,Scalar(255,255,255),-1);
//		circle(warp,Point(destinations[2].x,destinations[2].y),15,Scalar(255,255,255),1);
//		circle(warp,Point(destinations[3].x,destinations[3].y),3,Scalar(255,255,255),-1);
//		circle(warp,Point(destinations[3].x,destinations[3].y),40,Scalar(255,255,255),1);
		circle(warp,Point(destinations[count_of_dest].x,destinations[count_of_dest].y),3,Scalar(0,0,255),-1);
		circle(warp,Point(destinations[count_of_dest].x,destinations[count_of_dest].y),40,Scalar(0,0,255),1);
//Aby
//		time_for_rect = ((double)cvGetTickCount() - time_for_rect)/(1000.0*(double)cvGetTickFrequency());
//		rectangle(warp,oball.bounding_box,Scalar(0,0,255));
		
//		time_for_imshow = (double)cvGetTickCount();
		imshow("warp",warp);
//		time_for_imshow = ((double)cvGetTickCount() - time_for_imshow)/(1000.0*(double)cvGetTickFrequency());

//		time_for_loop = ((double)cvGetTickCount() - time_for_loop)/(1000.0*(double)cvGetTickFrequency());
		
//		printf("\nTime for cap : %lf\n",(double)time_for_cap);
//		printf("\nTime for resize : %lf\n",(double)time_for_resize);
//		printf("\nTime for warp : %lf\n",(double)time_for_warp);
//		printf("\nTime for init : %lf\n",(double)time_for_init);
//		printf("\nTime for bot : %lf\n",(double)time_for_bot);
//		printf("\nTime for imshow : %lf\n",(double)time_for_imshow);
//		printf("\nTime for cvt : %lf\n",(double)time_for_cvt);
//		printf("\nTime for rect : %lf\n",(double)time_for_rect);
		
//		out << warp;
//		cout<<bot.bot_angle<<endl;
	
		time_for_loop = ((double)cvGetTickCount() - time_for_loop)/(1000.0*(double)cvGetTickFrequency());
//		printf("\nTime for loop : %lf\n",(double)time_for_loop);
		
//		movement(0,socket, bot.bot_angle-180,0,20,integral_error, velocity);
		
//		out << warp;
		c = waitKey(1);
		
		if(c==27)
			break;
		if(c == ' ')
			c=waitKey(0);
	}
	delete[] pts;
	destroyAllWindows();
//	cap.release();
}	


