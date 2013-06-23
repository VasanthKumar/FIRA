#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>

#include "global_var.h"
#include "colors.h"
#include "contours.h"
#include "our_bot.h"
#include "update_frame.h"
#include "main.h"

using namespace cv;
using namespace std;

extern Point arena_center;

void limit_location_within_arena( Rect &location ){	

    if( location.x < 0 )
        location.x = 0;

    if( location.y < 0)
        location.y = 0;

    if( location.width + location.x > 640 )
        location.width = 640 - location.x;

    if( location.height + location.y > 480 )
        location.height = 480 - location.y;
}

void update_location( Rect &location, Point center ){
//    cout<<"center = "<<center.x<<' '<<center.y<<endl;
    location = Rect( center.x - BOUND_RECT, center.y - BOUND_RECT, 2 * BOUND_RECT, 2 * BOUND_RECT );
}

void expand_location( Rect &location ){
    location = Rect( location.x - BOUND_RECT, location.y - BOUND_RECT, location.width + 2 * BOUND_RECT, location.height + 2 * BOUND_RECT );
}

inline double distanc( Point pt1, Point pt2 ){
    return ( sqrt( ( pt1.x - pt2.x ) * ( pt1.x - pt2.x ) + ( pt1.y - pt2.y ) * ( pt1.y - pt2.y ) ) );
}

int angl( Point2f dst, Point2f cen, Point2f front ){

    float a = ( float )( ( dst.x - cen.x ) * ( front.y - cen.y ) - ( front.x - cen.x ) * ( dst.y - cen.y ) );	//angle through cross product.
    float b = ( float )( distanc( cen, dst ) * distanc( cen, front ) );
    float result = 0;
    float angle = 1000;

    if( b != 0 ){
        result = asin( a / b );

        if(distanc( cen, dst ) > distanc( front, dst ) ){
            angle = ( ( result * 90 / 1.57 ) );
        }
        else if( result > 0 ){
            angle= ( ( 180 - result * 90 / 1.57 ) );
        }
        else if( result < 0 ){
            angle = ( -180 - result * 90 / 1.57 );
        }
    }
    if( angle == 1000 ){
        if( front.y > cen.y )
            angle = 180;
        else if( cen.y > front.y )
            angle = 0;
        else
            printf( " < " );
    }
    return angle;
}

our_bot::our_bot(){
    x = 0, y = 0, angle = 0;
    mask = new Mat[3];
    location = Rect(arena_center.x - BOUND_RECT, arena_center.y - BOUND_RECT,
            2* BOUND_RECT, 2 * BOUND_RECT);
    mask[0] = Mat( Size(640,480), CV_8UC1 );
    mask[1] = Mat( Size(640,480), CV_8UC1 );
    mask[2] = Mat( Size(640,480), CV_8UC1 );
}	

our_bot::~our_bot(){
    mask[0].release();
    mask[1].release();
    mask[2].release();
    delete mask;
}

Rect goal_rect = Rect(0,480*65/180,640,480*50/180);

double our_bot::pos(){
    Point center;
    center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );


    x = ( ( center.x - arena_center.x ) *250 ) / goal_rect.width;		//center wrt the arena center.
    y=-1*((center.y-arena_center.y)*250)/goal_rect.width;
    
	bot_pos.x = x;
	bot_pos.y = y;
//	printf("botx=%f boty=%f\n",x,y);
}

double our_bot::orientation(){

    angle = angl( Point( 640, back_center.y ), back_center, front_center );

    if( angle < 0 )
        angle = -1 * angle;
    else if( angle == 1000)		//case where the bot is not found in the bounding box.
        angle = 0;
    else if( angle > 0 )
        angle = 360 - angle;
	
	bot_angle = angle;
//    printf("angle%f\n",angle);
}


void our_bot::FindCenter(){

    vector <int> Area_base;
    int Area_frontl=0;
    int Area_frontr=0;	
    vector <RotatedRect> BaseCenter = all_contours( mask[0], Area_base,0 );

    Point frontl_center = Point( 0, 0 );
    Point frontr_center = Point( 0, 0 );
    RotatedRect c1;

    RotatedRect c2;
    back_center = Point( 0, 0 );

    for( int i = 0; i < BaseCenter.size(); i++ ){

        if(Area_base[i] > BASE_AREA_THRESH) {
            c1 = closest_contour( mask[1], BaseCenter[i].center, Area_frontl );
            c2 = closest_contour( mask[2], BaseCenter[i].center, Area_frontr );


            if( c1.center.x != 0 && c2.center.x !=0 ){

                if(angl( c1.center,BaseCenter[i].center,c2.center) > 25 &&
                        distanc(c1.center,BaseCenter[i].center) < BOT_LENGTH && 
                        distanc(c2.center,BaseCenter[i].center) < BOT_LENGTH )
                {
                    back_center = BaseCenter[i].center;
                    frontl_center = c1.center;
                    frontr_center = c2.center;

                    break;
                }	

            }
        }
    }
    front_center = Point( ( frontl_center.x + frontr_center.x ) / 2, ( frontl_center.y + frontr_center.y ) / 2 );

}

void our_bot::update(){
    Mat gray_mask[3];

    gray_mask[0] = dst(location);
    gray_mask[1] = dst(location);
    gray_mask[2] = dst(location);

    mask[0] = Mat::zeros(gray_mask[0].rows,gray_mask[0].cols,CV_8UC1);
    mask[1] = Mat::zeros(gray_mask[1].rows,gray_mask[1].cols,CV_8UC1);
    mask[2] = Mat::zeros(gray_mask[2].rows,gray_mask[2].cols,CV_8UC1);

//    cout<<"before pickcolor\n";
    pick_color( gray_mask[0],mask[0], basecolor );
    pick_color( gray_mask[1],mask[1], lcolor );
    pick_color( gray_mask[2],mask[2], rcolor );
	
//	imshow("green",mask[0]);
//	imshow("pink",mask[1]);
//	imshow("yellow",mask[2]);
//	cout<<"color found\n";
    FindCenter();

    if( front_center.x != 0 && back_center.x != 0 ){
        //Taking care for the relative position changes due to ROI.
        front_center = Point( front_center.x + location.x, front_center.y + location.y );
        back_center = Point( back_center.x + location.x, back_center.y + location.y );
        bot_center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );
        update_location( location, bot_center );
//        cout<<"bot found!!"<<endl;
    }
    else{
        expand_location( location );
    }

    limit_location_within_arena( location );

//    cout<<"location = "<<location.x<<' '<<location.y
//        <<' '<<location.width<<' '<<location.height<<endl;
    pos();
    orientation();
}
