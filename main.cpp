#include "ball.h"

main(int argc,char** argv)
{
	VideoCapture cap;
	int f=0;
	Rect d;
	Mat s_image,s_image_gray,ROI,t,temp;
	
	cap.open(argv[1]);
	if(!cap.isOpened())
	{
		printf("\nCamera is not initialised properly, Aborting...\n");
	}
	
	Mat src_image = Mat::zeros(cap.get(3),cap.get(4),CV_8U);
	Mat dst = Mat::zeros(800,600,CV_8UC3);
	Mat dest = Mat::zeros(800,600,CV_8UC1);
	ball instance;
	
	while(1)
	{
		
		cap>>src_image;
		
		resize(src_image,s_image,Size(800,600));
		cvtColor(s_image,s_image_gray,CV_BGR2GRAY);
		resize(s_image,t,Size(800,600));
		resize(s_image_gray,temp,Size(800,600));
		
		t = Scalar(0,0,0);
		temp = Scalar(0);
		
		if(f==0)
		{
			d = Roi(s_image);
			f=1;
			instance.dbox = d;
		}
		
		dst = s_image(d);
		
		cvtColor(dst,dest,CV_BGR2GRAY);
		
		printf("\nDBOX:(%d,%d,%d,%d)\n",d.x,d.y,d.width,d.height);
		printf("\nBBOX:(%d,%d,%d,%d)\n",instance.bbox.x,instance.bbox.y,instance.bbox.width,instance.bbox.height);
		
		ROI = temp(instance.bbox);
		ROI = Scalar(255);
		ROI.release();
		s_image.copyTo(t,temp);
		
		instance.init(t);
		
		imshow("t",t);
		imshow("dest",dest);
		imshow("dst",dst);

		if(waitKey(5)==27)
		break;		
		
	}
	destroyAllWindows();
	cap.release();
}
