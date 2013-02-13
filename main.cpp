#include "arena.h"
#include "ball.h"


main(int argc,char** argv)
{
	VideoCapture cap;
	int f=0;
	
	cap.open(argv[1]);
	if(!cap.isOpened())
	{
		printf("\nCamera is not initialised properly, Aborting...\n");
	}
	
	Mat src_image = Mat::zeros(cap.get(3),cap.get(4),CV_8U);
	Mat dst = Mat::zeros(800,600,CV_8UC3);
	Mat warp = Mat::zeros(640,480,CV_8UC3);
	Mat warp_mat;
	Point2f* pts;
	pts = new Point2f [4];
	ball oball = ball();
	double time_for_warp = 0;
	double time_for_init = 0;
	double time_for_resize = 0;	
	while(1)
	{
		
		cap>>src_image;
		time_for_resize = (double)cvGetTickCount();
		resize(src_image,dst,Size(800,600));
		time_for_resize = ((double)cvGetTickCount() - time_for_resize)/(1000.0*(double)cvGetTickFrequency());
		
		if(f==1)
		{
			time_for_warp = (double)cvGetTickCount();
			warp = perspectiveArena(warp_mat,dst);
			time_for_warp = ((double)cvGetTickCount() - time_for_warp)/(1000.0*(double)cvGetTickFrequency());
			time_for_init= (double)cvGetTickCount();
			oball.init(warp);
			time_for_init = ((double)cvGetTickCount() - time_for_init)/(1000.0*(double)cvGetTickFrequency());
			imshow("warp",warp);
			
		}
		else if(f==0)
		{
			pts = selectPoint(dst);	
			warp_mat=getTransformMat(pts);	
			
			f=1;
			destroyAllWindows();
		}
		//warp.convertTo(warp,CV_8UC3,255);
		printf("\nTime for resize : %lf\n",(double)time_for_resize);
	printf("\nTime for warp : %lf\n",(double)time_for_warp);
	printf("\nTime for init : %lf\n",(double)time_for_init);
		
		//imshow("src",dst);
		
		
		
		if(waitKey(20)==27)
		break;
		
	}
	delete pts;
	destroyAllWindows();
	cap.release();
}	
	
