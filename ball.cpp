#include "ball.h"

ball::ball(){
    color = 'o';
    center = Point( 0, 0 );
    location = Rect(arena_center.x - BOUND_RECT, arena_center.y - BOUND_RECT,
            2* BOUND_RECT, 2 * BOUND_RECT);
    mask = new Mat[1];
    mask[0] = Mat( Size(image.cols, image.rows), CV_8UC1 );
}

void ball::FindCenter(){	
    int counter = 0;
    RotatedRect ball_rrect = largest_contour( mask[0], counter );
    if( ball_rrect.center.x != 0 )
        center = ball_rrect.center;
    else
        center = Point(0,0);

}

void ball::calculate_velocity()
{
    prev_velocity = velocity;
    if(center.x != 0 && prev_center.x != 0) {
        velocity.x = prev_center.x - center.x;
        velocity.y  = prev_center.y - center.y;
    }
    else {
        velocity = Point(1000, 1000);
    }
}

void ball::update(){
    
    Mat roi_image = image(location);
    
    GaussianBlur( roi_image, roi_image, Size( 3, 3 ), 0, 0 );
    
    cvtColor(roi_image, roi_image, CV_BGR2HSV);

    mask[0] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);
    
    pick_color( roi_image, mask[0], color );
    imshow("ball mask", mask[0]);

    prev_center = center;

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

    calculate_velocity();


}

