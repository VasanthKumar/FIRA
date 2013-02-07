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
	Point2f* pts;
	pts = new Point2f [4];
	ball oball = ball();
	while(1)
	{
		
		cap>>src_image;
		resize(src_image,dst,Size(800,600));
		 if(f==1)
		{
			warp = perspectiveArena(dst,pts);
			imshow("warp",warp);
			oball.init(warp);
		}
		else if(f==0)
		{
			pts = selectPoint(dst);		
			
			f=1;
		}
		
		//warp.convertTo(warp,CV_8UC3,255);
		imshow("src",dst);
		
		
		
		if(waitKey(20)==27)
		break;
		
	}
	delete pts;
	destroyAllWindows();
	cap.release();
}	
	
