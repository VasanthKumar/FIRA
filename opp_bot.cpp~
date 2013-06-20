#pragma once
#include <cv.h>
#include <highgui.h>

#include "opp_bot.h"
#include "update_frame.h"
#include "colors.h"
#include "contours.h"
#include "global_var.h"

using namespace  cv;
using namespace std;

int opp_bot::total_no_of_objects;

opp_bot o_bot[5];

opp_bot::opp_bot(){
    OBJECT_NUM = total_no_of_objects++;
    color = OPP_JERSEY_COLOR;
    center = Point( 0, 0 );
    location = Rect( 0, 0, 0, 0 );
    capture >> img;
   // cvShowImage("test",img);
    mask = Mat( Size(640,480), CV_8UC1 );
}


void opp_bot::FindCenter( Point &cen ){	
    vector <int> area;
    vector <Point> vector_cen = FindAllCenter( mask, area );
    cen = Point ( 0, 0 );
    int count = 0;

    for( int i = 0 ; i < vector_cen.size() ; i++ ){
        count = 0;
        for( int j = 0 ; j < OBJECT_NUM ; j++ ){
            if( (vector_cen[i].x + this->location.x != o_bot[j].center.x) || (vector_cen[i].y + this->location.y != o_bot[j].center.y)){
                count++;
            }
        }
        if( count == OBJECT_NUM ){
            cen = vector_cen[i];
            break;
        }
    }

}


void opp_bot::update(){
    Point cen;
	Mat mask_roi=mask(location);
//    cvSetImageROI( mask, location );
    
    pick_basecolor( mask_roi, location, color );
    FindCenter( cen );
//    cvResetImageROI( mask );
	mask_roi.release();

    if( cen.x != 0 ){
        cen = Point( cen.x + location.x, cen.y + location.y );
        center = cen;
        update_location( location, center );
    }
    else{
        expand_location( location );
    }

    limit_location_within_arena( location );

}


