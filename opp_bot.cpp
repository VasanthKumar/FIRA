#include "opp_bot.h"

int opp_bot::total_no_of_objects;

opp_bot::opp_bot(){
    OBJECT_NUM = total_no_of_objects++;
    color = OPP_JERSEY_COLOR;
    center = Point( 0, 0 );
    location = Rect(arena_center.x - BOUND_RECT, arena_center.y - BOUND_RECT,
            2* BOUND_RECT, 2 * BOUND_RECT);
    mask = new Mat[1];
    mask[0] = Mat( Size(image.cols, image.rows), CV_8UC1 );
}

void opp_bot::FindCenter(){	
    vector <int> area;
    vector <RotatedRect> opp_jersey_rrect = all_contours( mask[0], area, 0 );
    center = Point ( 0, 0 );
    int count = 0;

    for( int i = 0 ; i < opp_jersey_rrect.size() ; i++ ){
        count = 0;
        for( int j = 0 ; j < OBJECT_NUM ; j++ ){
            if( (opp_jersey_rrect[i].center.x + this->location.x != o_bot[j].center.x) || 
                    (opp_jersey_rrect[i].center.y + this->location.y != o_bot[j].center.y)){
                count++;
            }
        }
        if( count == OBJECT_NUM ){
            center = opp_jersey_rrect[i].center;
            break;
        }
    }
}

void opp_bot::update(){
   
    Mat roi_image = image(location);
    
    GaussianBlur( roi_image, roi_image, Size( 3, 3 ), 0, 0 );
    
    cvtColor(roi_image, roi_image, CV_BGR2HSV);

    mask[0] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);
    
    pick_color( roi_image, mask[0], color );
    imshow("opp bot mask", mask[0]);
    FindCenter();
    
    cvtColor(roi_image, roi_image, CV_HSV2BGR);

    if( center.x != 0 ){
        center = Point( center.x + location.x, center.y + location.y );
        update_location( location, center );
    }
    else{
        expand_location( location );
    }

    limit_location_within_arena( location );

}
