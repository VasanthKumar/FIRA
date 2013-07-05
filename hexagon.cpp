#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

#define radius 50

struct points
{
    Point point;
    struct points *next;
};

struct queue
{
    points *head, *list, *spare;
    
        queue()
        {
            head = NULL;
            list = NULL;
			spare = NULL;
        }
        
        void insert(Point pt)
        {
            points *new_point;
            new_point = new points;
            new_point->point = pt;
            list = head;
            if(head = NULL)
            {
                head = new_point;
                new_point->next = NULL;
            }
            else
            {
                new_point->next = list;
				list = new_point;
                head = new_point;
            }
        }    
        
        void display()
        {
            if(head == NULL)
            {
                cout<<"empty queue display\n";
                return;
            } 
            list = head;
            while(list!=NULL)
            {
                cout<<list->point<<"\n";
                list = list->next;
            }
        }
                
        void delete_one(Point pt)
        {
			int counter = 0;
            if(head == NULL)
            {
                cout<<"Empty queue_delete_one\n";
                return;
            }
            else 
            {
                list = head;
                while(list!= NULL)
                {
                    if(list->point == pt)
                    {
						counter = 1;
					    break;
					}	
                    list = list->next;    
                }   
                
                if(counter)
				{			
					if(head == list)
                	{
						spare = head;
						if(list->next ==  NULL)
							head = NULL;
						else
                    		head = head->next;
                	}
                	else
                	{	
                    	list = head;
                    
						while(list->next->point != pt)
                        	list = list->next;
					
						spare = list->next;
                   	
					 	if(spare->next == NULL)
							list->next = NULL;
						else
                    		list->next = spare->next;
                	}
            	}
			}
            delete(spare);
        }    
                
        void delete_all()
        {
            if(head == NULL)
                cout<<"Empty list delete_all\n";
            else
            {
                list = head;
                while(list->next != NULL)
                {
                    delete(list->next);        
                    list = list->next;
                }
                list = NULL;
             }
         }
		
		int search(Point pt)
		{
			list = head;
			while(list != NULL)
			{
				if(list->point == pt)
					return 1;
				list = list->next;
			}
			return 0;
		}	
};

float dist(Point pt1, Point pt2)
{
    float delta_x = (pt1.x - pt2.x);
    float delta_y = (pt1.y - pt2.y);
    float answer = sqrt((delta_x*delta_x) + (delta_y*delta_y));
    return (answer);
}    

int side_of_line(float m, float c, Point pt)
{
    float value = pt.y - m*pt.x - c;

    if(value>0.01)
        return 1; 
    else if(value<-0.01)
        return -1;
    else 
        return 0;    
}

int detect_obstacle(Point pt1, Point pt2, Point hexagon[6])
{
    int test[3] = {0,0,0} , answer,i;
    
    float m = ((float)pt2.y - pt1.y)/(pt2.x - pt1.x);
    float c = (pt1.y - m*pt1.x);
    
    if((pt1.x - pt2.x) == 0)
    {
        for(i = 0; i<3; i++)
        {
            test[i] = (hexagon[i].x-pt1.x)*(hexagon[i+3].x-pt1.x);
        }           
    }           
    else
    {   
        for(i = 0; i<3; i++)
            test[i] = side_of_line(m,c,hexagon[i])*side_of_line(m,c,hexagon[i+3]);
    }    
    if( test[0]<0 || test[1]<0 || test[2]<0 )
        answer = 1;
    else
        answer = 0;    
    
    return answer; 
}

Point closest_point(Point source, points* head_open, Point dest)
{
    float distance = 65536.0;
    Point closest;
    float dist0 = 0, dist1 = 0, dist2 = 0;
    
    while((head_open) != NULL)
    {   
        dist0 = dist(source,(head_open)->point);
        if(dist0 <= distance*1.02 && ((head_open)->point != source))
        {
            if(abs(dist(source,(head_open)->point) - distance)<25)
            {
                dist1 = dist(dest,closest);
                dist2 = dist(dest,(head_open)->point);
                if(dist1>dist2)
                    closest = (head_open)->point;
            }
            else
            {    
            distance = dist(source,(head_open)->point);
            closest = (head_open)->point;
            }
        }
        (head_open) = (head_open)->next;
    }
    return closest;
}

void Myline(Mat image,Point pt1, Point pt2)
{
    int thickness = 1;
    int lineType = 8;
    
    line( image , pt1, pt2, Scalar(0,0,255), thickness, lineType);        
}
    
void MyHexagon(Mat image,Point pt, Point array[6])
{
    array[0] = Point(pt.x + radius, pt.y);
    array[1] = Point(pt.x + radius/2, pt.y + (radius*0.866));
    array[2] = Point(pt.x - radius/2, pt.y + (radius*0.866));
    array[3] = Point(pt.x - radius, pt.y);
    array[4] = Point(pt.x - radius/2, pt.y - (radius*0.866));
    array[5] = Point(pt.x + radius/2, pt.y - (radius*0.866));
               
    int thickness = 1;
    int lineType = 8;

    line( image , Point(pt.x + radius, pt.y), Point(pt.x + radius/2, pt.y + (radius*0.866)), Scalar(0,0,0), thickness, lineType);    
    line( image , Point(pt.x + radius/2, pt.y + (radius*0.866)), Point(pt.x - radius/2, pt.y + (radius*0.866)), Scalar(0,0,0), thickness, lineType);    
    line( image , Point(pt.x - radius/2, pt.y + (radius*0.866)), Point(pt.x - radius, pt.y), Scalar(0,0,0), thickness, lineType);    
    line( image , Point(pt.x - radius, pt.y), Point(pt.x - radius/2, pt.y - (radius*0.866)), Scalar(0,0,0), thickness, lineType);    
    line( image , Point(pt.x - radius/2, pt.y - (radius*0.866)), Point(pt.x + radius/2, pt.y - (radius*0.866)), Scalar(0,0,0), thickness, lineType);    
    line( image , Point(pt.x + radius/2, pt.y - (radius*0.866)), Point(pt.x + radius, pt.y), Scalar(0,0,0), thickness, lineType);     
}

int main( void )
{
    Point hex[8] = { Point(150,150), Point(300,150),Point(450,150), Point(600,150),Point(150,300), Point(300,300),Point(450,300), Point(600,300)} ;
	Point source = Point(100,100), dest = Point(700,400),closest;
	Point bound_1 = Point(100,100), bound_2 = Point(1200,500);

    Point around[6];
    queue open,close;
    int i,j;
	float m,c;
    
	if(dest.y != source.y)
		m = ((float)source.x - dest.x)/(dest.y - source.y),c;
	
	char hexagon_window[] = "Drawing: hexagons";
    Mat image = Mat( 600, 1300, CV_8UC3, Scalar(255,255,255));
	
    Myline(image,source,dest);

	for(i = 0; i<8; i++)
    {
		MyHexagon( image,hex[i],around );
	 
  	  	if(detect_obstacle(source,dest,around))
    	{
        	for(j=0;j<6;j++)
			{
				if(!((around[j].x <= bound_1.x)||(around[j].y <= bound_1.y)||(around[j].x >= bound_2.x)||(around[j].y >= bound_2.y)))
            		open.insert(around[j]);  
			}
    	}
    }    
   
	if(!((dest.x <= bound_1.x)||(dest.y <= bound_1.y)||(dest.x >= bound_2.x)||(dest.y >= bound_2.y)))	
		open.insert(dest); 
	close.insert(source);
	
	if(open.head != NULL)
    {
        closest = source;
        while(open.head != NULL)
        {
            closest = closest_point(closest,open.head,dest);
            close.insert(closest);
			c = closest.y - m*closest.x;
			
			open.spare = open.head;	
			while(open.spare != NULL)
			{
				if(dest.y != source.y)
				{
					if((side_of_line(m,c,dest)*side_of_line(m,c,open.spare->point))<=0)
						open.delete_one(open.spare->point);
					open.spare = (open.spare)->next;
				}
				else
				{
					if(((closest.x-dest.x)*(closest.x-open.spare->point.x))<=0)
						open.delete_one(open.spare->point);
					open.spare = (open.spare)->next;
				}
			}
		}
    }
   	
	//cout<<"open list modified\n";
	//open.display();
	//cout<<"closed list\n";
	//close.display();

    close.list = close.head;
    while((close.list)->next != NULL)
    {    
        line( image , (close.list)->point, (close.list)->next->point, Scalar(255,0,0), 1, 8);                
        (close.list) = (close.list)->next;
    }
    
	if(!close.search(dest))
		cout<<"No route to destination found\n";
    close.delete_all();        
    open.delete_all();
    
    imshow( hexagon_window, image );
 
    waitKey( 0 );
    return(0);
}

