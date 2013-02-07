#include "arena.h"


Mat imageR;
Mat imageC;
int flag = 0,count=0,flagc=0;
bool drawing_box = false;
bool sel = false;
Rect box;
Point2f point;

void draw_box( Mat img, Rect rect ){

	rectangle( img,rect,cvScalar(0xff,0x00,0x00) );
}

void draw_circle( Mat img, Point2f center, int radius ){

	circle( img,center,radius,cvScalar(0x00,0xff,0x00),CV_FILLED );
}

static void onMouseRect( int event, int x, int y, int, void* )
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
			draw_box( imageR, box );
			
 			flag=1;
 			
			break;
		}
}

Rect selectRect( Mat argm )
{
    Mat image0 , ROI;
    Rect b;
    char c;
    
    argm.copyTo(image0);
    
    if( image0.empty() )
    {
       printf("Image empty\n");
        return Rect();
    }
 
    image0.copyTo(imageR);

    namedWindow( "image", 1 );
    setMouseCallback( "image", onMouseRect, 0 );
    
    while(1)
    {	
  
    	if(drawing_box)
    	{
    		draw_box(imageR,box);
    	}
    	
    	if(flag==1)
    	{
    		
    		ROI = imageR(box);
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
    			image0.copyTo(imageR);
    			
    			flag = 0;
    		}
    	}
    	
    	imshow("image",imageR);
    	
    	if(waitKey(15)==27)
    	return Rect();
    	
    	image0.copyTo(imageR);
    	
     }	
     
     b = box;
    
    return b;
}

static void onMouseSelect( int event, int x, int y, int, void* )
{
    switch( event ){
		case CV_EVENT_MOUSEMOVE: 
			{
				if(sel)
				point = Point2f((float)x,(float)y);
				break;
			}		
		case CV_EVENT_LBUTTONDOWN:
			{
				point = Point2f((float)x,(float)y);
				sel = true;
				break;
			}
		case CV_EVENT_LBUTTONUP:
			{
				sel = false;
				point = Point2f((float)x,(float)y);
				draw_circle(imageC,point,10);
				count++;
				flagc=1;
				break;
			}
		}
}

Point2f* selectPoint(Mat src)
{
    Point2f* points;
    points = (Point2f*)malloc(sizeof(Point2f)*4);
    char c;
    
    if( src.empty() )
    {
       printf("Image empty\n");
        return NULL;
    }
    
    src.copyTo(imageC);

    namedWindow( "SelectPoints", 1 );
    count = 0;
    setMouseCallback( "SelectPoints", onMouseSelect, 0 );
    
    while(count<5)
    {	
  
    	if(flagc==1)
    	{
    	
    		imshow("SelectPoints",imageC);
    		c = waitKey(0);
    		if(c=='c')
    		{
    			points[count-1] = point; 
    			printf("\nCount:%d\n",count);
    			if(count == 4)
    			break;
    		}
    		else
    		{
    			count--;
    			printf("\nCount:%d",count);
    			src.copyTo(imageC);
    			
    		}
    		flagc = 0;
    	}
    	
    	imshow("SelectPoints",imageC);
    	
    	if(waitKey(15)==27)
    	return NULL;
    	
    	
    	//src.copyTo(imageC);
    	
     }	
    printf("\nReturning\n");
    return points;

}

Mat perspectiveArena(Mat src, Point2f* srcq )
{
	Point2f srcQuad[4],dstQuad[4];
	Mat warped;
	Mat warp_perspective;
	
	srcQuad[0] = srcq[0];
	srcQuad[1] = srcq[1];
	srcQuad[2] = srcq[2];
	srcQuad[3] = srcq[3];
	
	dstQuad[0] = Point2f(192.0/5.0,0.0);
	dstQuad[1] = Point2f(3008.0/5.0,0.0);
	dstQuad[2] = Point2f(3008.0/5.0,480.0);
	dstQuad[3] = Point2f(192.0/5.0,480.0);
	
	warp_perspective= getPerspectiveTransform(srcQuad,dstQuad);
	warpPerspective(src, warped, warp_perspective, Size(640,480));
	
	return warped;
	
}


