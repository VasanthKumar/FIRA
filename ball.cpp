#include"ball.h"

vector<vector<Point> > contours;

vector<Vec4i> hierarchy;


void ball::hueRange (Mat src, int hue_lower, int hue_upper,int flag=0){

	printf("\nEntering hueRange\n");
	if(src.depth()!=CV_8U)
	{
		printf("\nError in function hueRange. Function supports only CV_8U. Exiting.. ");
	}
	
	Mat temp_image = Mat::zeros(src.cols, src.rows, CV_8U);
	Mat temp_image_gray = Mat::zeros(src.cols, src.rows, 1);
	Mat temp_image_return = Mat::zeros(src.cols,src.rows,CV_8U);
	Mat element = getStructuringElement(MORPH_RECT,Size( 2*1 + 1, 2*1+1 ),Point( 1,1) );
	
	cvtColor(src,temp_image,CV_RGB2HSV);
	inRange(temp_image,Scalar(hue_lower,20,20),Scalar(hue_upper,255,255),temp_image_gray);
	
	morphologyEx(temp_image_gray,temp_image_gray,2,element);
	morphologyEx(temp_image_gray,temp_image_gray,3,element);
	
	//morphologyEx(temp_image_gray,temp_image_gray,2,element1);
	
	
	if(flag==0)
	{
		temp_image_gray.copyTo(ball_image);
	}
	else
	{
		src.copyTo(temp_image_return,temp_image_gray);
		temp_image_return.copyTo(ball_image);
	}
	imshow("ball_image",ball_image);
	//printf("\nBallimage depth : %d",ball_image.channels());
	//waitKey(0);
		printf("\nLeaving hueRange\n");
}

void ball::con(int flag=0)			//Rect
{
		printf("\nEntering con\n");
	Mat temp = Mat(ball_image.cols,ball_image.rows,CV_8UC1);
	
	Mat display = Mat::zeros(temp.cols,temp.rows,CV_8UC3);
	
	
	if(ball_image.channels()==3)
	{
		cvtColor(ball_image,temp,CV_RGB2GRAY);
	}
	else if(ball_image.channels()==1)
	{
		ball_image.copyTo(temp);
	}	
	 
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE); //fn to find contours
	
	int i=0;
	
	int counter = 0;
	vector<Rect>boundrect(contours.size());
		
	while(i<contours.size())
	{
	
		if(contourArea(contours[i])>50&&contourArea(contours[i])<400&&hierarchy[i][3]<0) //constraints on size
			{
				boundrect[i] = boundingRect(contours[i]);
				//rectangle(ball_image,boundrect[i].tl(),boundrect[i].br(),Scalar(255));
				rectangle(display,boundrect[i].tl(),boundrect[i].br(),Scalar(255,0,0));
				counter++;
		
			}
		i++;
		
	}
	//printf("\n%d,%d\n",i,counter);
	
	imshow("display",display);
	imshow("temp",temp);
		printf("\nLeaving con \n");		
}	

void ball::calc_velocity()
{
	vel_ball.x = pos_ball_centre.x - pos_ball_centre_prev.x;
	vel_ball.y = pos_ball_centre.y - pos_ball_centre_prev.y;
	printf("\n(vx,vy)=(%f,%f)\n",vel_ball.x,vel_ball.y);
}

ball::ball(Mat src)
{
	pos_ball_centre = Point2f(0.0,0.0);
	pos_ball_centre_prev = Point2f(0.0,0.0);
	calc_velocity();
	bbox = Rect(0.0,0.0,0.0,0.0);
	hueRange(src,108,118,0);
	con();
}
ball::ball()
{
	pos_ball_centre = Point2f(0.0,0.0);
	pos_ball_centre_prev = Point2f(0.0,0.0);
	calc_velocity();
	bbox = Rect(0.0,0.0,0.0,0.0);
	ball_image = Mat(Width,Height,Depth);
	
}


