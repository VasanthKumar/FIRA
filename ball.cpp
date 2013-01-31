#include "ball.h"

 vector<vector<Point> > contours;
 vector<Vec4i> hierarchy;
 
Mat ball::getMask(Mat src,int hue_lower,int hue_upper)
{
	printf("\nEntering getMask\n");
	
	Mat temp_image = Mat::zeros(src.cols,src.rows,CV_8UC3);
	Mat temp_image_gray = Mat::zeros(src.cols,src.rows,CV_8UC1);
	printf("\n%d %d\n",src.depth(),src.type());
	Mat element = getStructuringElement(MORPH_RECT,Size( 2*1 + 1, 2*1+1 ),Point( -1,-1) );
	
	cvtColor(src,temp_image,CV_BGR2HSV);
	
	inRange(temp_image,Scalar(hue_lower,30,0),Scalar(hue_upper,255,255),temp_image_gray);
	
	//morphologyEx(temp_image_gray,temp_image_gray,2,element);

	printf("\nLeaving getMask\n");
	return temp_image_gray;
}

void ball::findPosition(int flag=0)
{
	printf("\nEntering findPosition\n");
	Mat temp = Mat(mask.cols,mask.rows,CV_8UC1);
	Mat display = Mat::zeros(temp.cols,temp.rows,CV_8UC3);
	mask.copyTo(temp);
	imshow("temp",temp);
	printf("\nBBOX:(%d,%d,%d,%d)\n",bounding_box.x,bounding_box.y,bounding_box.width,bounding_box.height);
	printf("\nVelocity:(%f,%f)\n",velocity.x,velocity.y);
	Mat t = temp(bounding_box);
	imshow("t",t);
	findContours(t,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,Point2f(bounding_box.x,bounding_box.y));
	
	int i=0,counter=0,pos=-1,posarea=-1;
	vector<Rect>boundrect(contours.size());
	double maxarea=0;
		
	while(i<contours.size())
	{
	
		if(contourArea(contours[i])>50&&contourArea(contours[i])<600&&hierarchy[i][3]<0) //constraints on size
			{
				boundrect[i] = boundingRect(contours[i]);
			 	rectangle(display,boundrect[i].tl(),boundrect[i].br(),Scalar(255,0,0));
				counter++;
				pos = i;
				if(contourArea(contours[i])>maxarea)
				{
					maxarea = contourArea(contours[i]);
					posarea = i;
					
				}
			}	
		i++;
	}
	
	if(counter==1)
	{
	   bounding_box.x = boundrect[posarea].x - boundrect[posarea].width/2;
	   bounding_box.y = boundrect[posarea].y - boundrect[posarea].height/2;
	   bounding_box.width = boundrect[posarea].width*2;
	   bounding_box.height = boundrect[posarea].height*2;
	   
	   prev_center = center;
	   center.x = bounding_box.x + boundrect[posarea].width;
	   center.y = bounding_box.y + boundrect[posarea].height; 
	   calculate_velocity();
	   
	   bounding_box.x = bounding_box.x + velocity.x;
	   bounding_box.y = bounding_box.y + velocity.y;
	   bounding_box.width+= fabs(velocity.x);
	   bounding_box.height+= fabs(velocity.y);
	   
	   if(bounding_box.x<0)
	   {
	   	bounding_box.x=0;
	   }
	    if(bounding_box.y<0)
	   {
	   	bounding_box.y=0;
	   }
	    if(bounding_box.x>main_image.cols)
	   {
	   	bounding_box.x=0;
	   	bounding_box.width=main_image.cols;
	   }
	   if(bounding_box.y>main_image.rows)
	   {
	   	bounding_box.y=0;
	   	bounding_box.height=main_image.rows;
	   }
	  
	   if(bounding_box.x+bounding_box.width>main_image.cols)
	   {
	   	bounding_box.x=main_image.cols-bounding_box.width;
	   }
	    if(bounding_box.y+bounding_box.height>main_image.rows)
	   {
	   	bounding_box.y=main_image.rows-bounding_box.height;
	   }
	   
		
	}
	else
	{
		bounding_box.x=0;
		bounding_box.y=0;
		bounding_box.width=Width;
		bounding_box.height=Height;
		prev_center.x = prev_center.x + velocity.x;
		prev_center.y = prev_center.y + velocity.y; 
			
	}
	
	imshow("display",display);
	
}
void ball::calculate_velocity()
{
	velocity.x = center.x - prev_center.x;
	velocity.y = center.y - prev_center.y;
}

ball::ball()
{
	main_image = Mat(Width,Height,CV_8U);
	
	center = Point2f(0.0,0.0);
	prev_center = Point2f(0.0,0.0);
	calculate_velocity();
	bounding_box = Rect(0,0,Width,Height);
	
}

void ball::init(Mat src)
{
	printf("\nEntering init\n");
	src.copyTo(main_image);
	
	mask= getMask(main_image,5,10);
	//waitKey(0);
	
	//mask = temp(bounding_box);
	
	findPosition();
	printf("\nLeaving init\n");
}

void ball::display_ball_prop()
{
	printf("\nCenter:(%f,%f)\t",center.x,center.y);
	printf("\nPrevCentre:(%f,%f)\n",prev_center.x,prev_center.y);
	printf("\nVelocity:(%f,%f)\n",velocity.x,velocity.y);
	printf("\nBBOX:(%d,%d,%d,%d)\n",bounding_box.x,bounding_box.y,bounding_box.width,bounding_box.height);
	
}
void ball::update()
{
	
}
