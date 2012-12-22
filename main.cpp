#include "ball.h"

main(int argc,char** argv)
{
	VideoCapture cap;
	cap.open(argv[1]);

	if(!cap.isOpened())
	{
		printf("\nCamera is not initialised properly, Aborting...\n");
	}
	
	Mat src_image = Mat::zeros(cap.get(3),cap.get(4),CV_8U);
	Mat dst = Mat::zeros(800,600,3);
	//Mat dst_color = Mat::zeros(src_image.cols,src_image.rows,CV_8UC3);
	ball instance;
	while(1)
	{
		cap>>src_image;
		resize(src_image,dst,Size(800,600));	
		instance = ball(dst);
		imshow("src",src_image);
		if(waitKey(20)==27)
		break;
		
	}
	destroyAllWindows();
	cap.release();
}
