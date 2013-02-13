#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>

#include "global_var.h"
#include "process_image.h"
#include "colors.h"
#include "contours.h"
#include "our_bot.h"
#include "update_frame.h"

using namespace cv;
using namespace std;

our_bot bot[5];

our_bot::our_bot(){
    x = 0, y = 0, angle = 0;
    mask = new Mat[3];
    location = Rect(arena_center.x - BOUND_RECT, arena_center.y - BOUND_RECT,
            2* BOUND_RECT, 2 * BOUND_RECT);
    mask[0] = Mat( Size(640,480), CV8UC1 );
    mask[1] = Mat( Size(640,480), CV8UC1 );
    mask[2] = Mat( Size(640,480), CV8UC1 );

}	

our_bot::~our_bot(){
    mask[0].release();
    mask[1].release();
    mask[2].release();
    delete mask;
}


double our_bot::pos(){
    Point center;
    center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );


    x = ( ( center.x - arena_center.x ) *250 ) / goal_rect.width;		//center wrt the arena center.
    y=-1*((center.y-arena_center.y)*250)/goal_rect.width;
    //printf("botx=%f boty=%f\n",x,y);
}

double our_bot::orientation(){

    angle = angl( Point( 640, back_center.y ), back_center, front_center );

    if( angle < 0 )
        angle = -1 * angle;
    else if( angle == 1000)		//case where the bot is not found in the bounding box.
        angle = 0;
    else if( angle > 0 )
        angle = 360 - angle;

    //printf("angle%f\n",angle);
}


void our_bot::FindCenter(){

    vector <int> Area_base;
    int Area_frontl=0;
    int Area_frontr=0;	
    vector <Point> BaseCenter = FindAllCenter( mask[0], Area_base );

    Point frontl_center = Point( 0, 0 );
    Point frontr_center = Point( 0, 0 );
    Point c1 = Point( 0 ,0 );
    Point c2 = Point( 0 ,0 );
    back_center = Point( 0, 0 );

    for( int i = 0; i < BaseCenter.size(); i++ ){

	printf(" Area of base %d ",Area_base[i]);        //FOR Debugging AREA threshold

	if(Area_base[i] > BASE_AREA_THRESH) {
        c1 = ClosestFrontCenter( mask[1], BaseCenter[i], Area_frontl );
	c2 = ClosestFrontCenter( mask[2], BaseCenter[i], Area_frontr );
      
	if( c1.x != 0 && c2.x !=0 ){

			if(angl(c1,BaseCenter[i],c2) > 0)
			{back_center = BaseCenter[i];
			frontl_center = c1;
			frontr_center = c2;

			break;
			}	
      
      }
    }
}
 front_center = Point( ( frontl_center.x + frontr_center.x ) / 2, ( frontl_center.y + frontr_center.y ) / 2 );

}


void our_bot::update(){
    //Setting the tracking box as the region of interest.
    mask_roi0=mask[0](location);
    mask_roi1=mask[1](location);
    mask_roi2=mask[2](location);
//    cvSetImageROI( mask[0], location );
//    cvSetImageROI( mask[1], location );
//    cvSetImageROI( mask[2], location );

    pick_basecolor( mask_roi0, location, basecolor );
    pick_frontcolor( mask_roi1, location, lcolor );
    pick_frontcolor( mask_roi2, location, rcolor );

    //The cvShowImage lines are for debuging.
    //cvShowImage( "back_center", mask[0] );
    //cvShowImage( "front_center_left", mask[1] );
    //cvShowImage( "front_center_right", mask[2]);

    FindCenter();

//    cvResetImageROI( mask[0] );
//    cvResetImageROI( mask[1] );
//    cvResetImageROI( mask[2] );
	  mask_roi0.release();
	  mask_roi1.release();
	  mask_roi2.release();

    if( front_center.x != 0 && back_center.x != 0 ){
        //Taking care for the relative position changes due to ROI.
        front_center = Point( front_center.x + location.x, front_center.y + location.y );
        back_center = Point( back_center.x + location.x, back_center.y + location.y );
        bot_center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );
        update_location( location, bot_center );
    }
    else{
        expand_location( location );
    }

    limit_location_within_arena( location );
    pos();
    orientation();
}
