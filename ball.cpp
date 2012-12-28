#include"ball.h"
#include<iostream>
vector<vector<Point> > contours;

vector<Vec4i> hierarchy;

bool drawing_box = false;
Rect box;
Mat image;
int flag = 0;

void draw_box( Mat img, Rect rect ){

	rectangle( img, rect,cvScalar(0xff,0x00,0x00) );
}

double dist( Point2f a,Point2f b ){

	double t =0 ;
	t = pow((a.x-b.x),2)+pow((a.y-b.y),2);
	t = pow(t,0.5);
	return t;
}


static void onMouse( int event, int x, int y, int, void* )
{
    switch( event ){
		case CV_EVENT_MOUSEMOVE: 
			if( drawing_box ){
				box.width = x-box.x;
				box.height = y-box.y;
			}
			break;

		case CV_EVENT_LBUTTONDOWN:
			drawing_box = true;
			box = cvRect( x, y, 0, 0 );
			break;

		case CV_EVENT_LBUTTONUP:
			drawing_box = false;
			
			if( box.width < 0 ){
				box.x += box.width;
				box.width *= -1;
			}
			if( box.height < 0 ){
				box.y += box.height;
				box.height *= -1;
			}
			box.x+=1;
			box.y+=1;
			box.width-=1;
			box.height-=1;
			draw_box( image, box );
			
 			flag=1;
 			
			break;
		}
}

Rect Roi( Mat argm )
{
    Mat image0;
    argm.copyTo(image0);
    Rect b;
    
    if( image0.empty() )
    {
       printf("Image empty\n");
        return Rect();
    }
  image0.copyTo(image);
  Mat ROI;
  char c;
    namedWindow( "image", 1 );

    setMouseCallback( "image", onMouse, 0 );
    while(1)
    {	
  
    	if(drawing_box)
    	{
    		draw_box(image,box);
    	}
    	if(flag==1)
    	{
    		
    		ROI = image(box);
    		imshow("ROI",ROI);
    		c = waitKey(0);
    		if(c=='c')
    		{
    			break;
    		}
    		else
    		{
    			ROI.release();
    			destroyWindow("ROI");
    			image0.copyTo(image);
    			
    			flag = 0;
    		}
    	}
    	imshow("image",image);
    	if(waitKey(15)==27)
    	return Rect();
    	image0.copyTo(image);
    	
     }	
     b = box;
    
    return b;
}

Mat ball::hueRange (Mat src, int hue_lower, int hue_upper,int flag=0){

	printf("\nEntering hueRange\n");
	if(src.depth()!=CV_8U)
	{
		printf("\nError in function hueRange. Function supports only CV_8U. Exiting.. ");
	}
	
	Mat temp_image = Mat::zeros(src.cols, src.rows, CV_8U);
	Mat temp_image_gray = Mat::zeros(src.cols, src.rows,CV_8U);
	Mat temp_image_return = Mat::zeros(src.cols,src.rows,CV_8U);
	Mat element = getStructuringElement(MORPH_RECT,Size( 2*1 + 1, 2*1+1 ),Point( 1,1) );
	
	cvtColor(src,temp_image,CV_BGR2HSV);
	inRange(temp_image,Scalar(hue_lower,20,20),Scalar(hue_upper,255,255),temp_image_gray);
	
	morphologyEx(temp_image_gray,temp_image_gray,2,element);

	printf("\nLeaving hueRange\n");
	if(flag==0)
	{
		return temp_image_gray;
	}
	else
	{
		src.copyTo(temp_image_return,temp_image_gray);
		return temp_image_return;
	}
	
		
}

void ball::con(int flag=0)			//Rect
{
		printf("\nEntering con\n");
	Mat temp = Mat(ball_image.cols,ball_image.rows,CV_8UC1);
	
	Mat display = Mat::zeros(temp.cols,temp.rows,CV_8UC3);
	
	
	if(ball_image.channels()==3)
	{
		cvtColor(ball_image,temp,CV_BGR2GRAY);
	}
	else if(ball_image.channels()==1)
	{
		ball_image.copyTo(temp);
	}
	
	imshow("temp",temp);
	
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);
	
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
	
	if(counter>0)
	{
	  
	   bbox.x = boundrect[posarea].x - boundrect[posarea].width/2;
	   bbox.y = boundrect[posarea].y - boundrect[posarea].height/2;
	   bbox.width = boundrect[posarea].width*2;
	   bbox.height = boundrect[posarea].height*2;
	   pos_ball_centre_prev = pos_ball_centre;
	   pos_ball_centre.x = bbox.x + boundrect[posarea].width/2;
	   pos_ball_centre.y = bbox.y + boundrect[posarea].height/2; 
	   circle(display,pos_ball_centre+vel_ball,10,Scalar(0,255,0),CV_FILLED);
	   
	   printf("\nPredCentre:(%f,%f)\t",pos_ball_centre_prev.x+vel_ball.x,pos_ball_centre_prev.y+vel_ball.y); 
	   printf("Centre:(%f,%f)\n",pos_ball_centre.x,pos_ball_centre.y);
	   calc_velocity();
	  
	   
	    if((bbox.width>dbox.width)||(bbox.height>dbox.height)) 
	   {
	   	bbox = dbox; 
	   }
	   
	  else
	  {
	  	  bbox.x = bbox.x + vel_ball.x;
	 	  bbox.y = bbox.y + vel_ball.y;
	 	  bbox.width+= fabs(vel_ball.x);
	 	  bbox.height+= fabs(vel_ball.y);
	 	  
	 	  if(bbox.x<0)
	 	  {
	 	  	bbox.x=0;
	 	  }
	 	  if(bbox.y<0)
	 	  {
	 	  	bbox.y=0;
	 	  }
	 	  if(bbox.x+bbox.width>ball_image.cols)
	 	  {
	 	  	bbox.x = ball_image.cols - bbox.width;
	 	  }
	 	  
	 	  if(bbox.y+bbox.height>ball_image.rows)
	 	  {
	 	  	bbox.y = ball_image.rows - bbox.height;
	 	  }
	 	  
	 	  if((bbox.width>dbox.width)||(bbox.height>dbox.height))
	 	  {
	 	  	bbox = dbox;
	 	  } 
	  }
	   //disp_ball_prop(); 		
	}
	
	else
	{
	    bbox = dbox;
          //  pos_ball_centre_prev = pos_ball_centre;
	    vel_ball = Point(0.0,0.0);	
	}
	
	printf("\n%d\n",counter);
	
	imshow("display",display);
	
		printf("\nLeaving con \n");	
			
}	

void ball::calc_velocity()
{
	vel_ball.x = pos_ball_centre.x - pos_ball_centre_prev.x;
	vel_ball.y = pos_ball_centre.y - pos_ball_centre_prev.y;
}

void ball::init(Mat src)
{
	
	src.copyTo(main_image);
	ball_image = hueRange(src,4,12,0);
	con();
	
}
ball::ball()
{
	pos_ball_centre = Point2f(0.0,0.0);
	pos_ball_centre_prev = Point2f(0.0,0.0);
	calc_velocity();
	dbox = Rect(0,0,800,600);
	bbox = Rect(0,0,800,600);
	
}
void ball::update()
{
	calc_velocity();
		
}
void ball::disp_ball_prop()
{
	
	printf("\nCentre:(%f,%f)\t",pos_ball_centre.x,pos_ball_centre.y);
	printf("\nPrevCentre:(%f,%f)\n",pos_ball_centre_prev.x,pos_ball_centre_prev.y);
	printf("\nVelocity:(%f,%f)\n",vel_ball.x,vel_ball.y);
	printf("\nBBOX:(%d,%d,%d,%d)\n",bbox.x,bbox.y,bbox.width,bbox.height);
	printf("\nDBOX:(%d,%d,%d,%d)\n",dbox.x,dbox.y,dbox.width,dbox.height);
}


