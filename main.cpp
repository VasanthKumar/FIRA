#include "arena.h"
#include "ball.h"
#include "our_bot.h"
#include "opp_bot.h"

using namespace cv;
using namespace std;

Mat dst;

Point arena_center;

main(int argc,char** argv)
{

    arena_center = Point(320,240);
	VideoCapture cap;
	int f=0;

	cap.open(argv[1]);
	if(!cap.isOpened())
	{
		printf("\nCamera is not initialised properly, Aborting...\n");
	}
	
	Mat src_image = Mat::zeros(cap.get(3),cap.get(4),CV_8U);
	dst = Mat::zeros(640,480,CV_8UC3);
    src_image.copyTo(dst);
	Mat warp = Mat::zeros(640,480,CV_8UC3);
	Mat warp_mat;
	Point2f* pts;
	pts = new Point2f [4];
	ball oball = ball();
	
    char c = 0;
    our_bot bot;

    double time_for_warp = 0;
	double time_for_init = 0;
	double time_for_resize = 0;	
	while(1)
	{	
		cap>>src_image;
		time_for_resize = (double)cvGetTickCount();
		resize(src_image,dst,Size(640,480));
        time_for_resize = ((double)cvGetTickCount() - time_for_resize)/(1000.0*(double)cvGetTickFrequency());


		if(f==1)
		{
			time_for_warp = (double)cvGetTickCount();
			warp = perspectiveArena(warp_mat,dst);
			time_for_warp = ((double)cvGetTickCount() - time_for_warp)/(1000.0*(double)cvGetTickFrequency());
			time_for_init= (double)cvGetTickCount();
		//	oball.init(warp);
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
        
        bot.basecolor = 'y';
        bot.lcolor = 'g';
        bot.rcolor = 'g';

        bot.update();

        rectangle(warp,Point(bot.bot_center.x-15,bot.bot_center.y-15),
                Point(bot.bot_center.x+15,bot.bot_center.y+15), Scalar(0,0,255));
        imshow("warp",warp);

        c = waitKey(1);
		if(c==27)
		break;
        if(c == ' ')
            c=waitKey(0);
		
	}
	delete pts;
	destroyAllWindows();
	cap.release();
}	
	
