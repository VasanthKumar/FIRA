#include "geometry.h"

inline double distanc( Point2f pt1, Point2f pt2 ){
    return ( sqrt( ( pt1.x - pt2.x ) * ( pt1.x - pt2.x ) + ( pt1.y - pt2.y ) * ( pt1.y - pt2.y ) ) );
}

void update_location( Rect &location, Point center ){
    location = Rect( center.x - BOUND_RECT, center.y - BOUND_RECT, 2 * BOUND_RECT, 2 * BOUND_RECT );
}

void expand_location( Rect &location ){
    location = Rect( location.x - BOUND_RECT, location.y - BOUND_RECT, location.width + 2 * BOUND_RECT, location.height + 2 * BOUND_RECT );
}

void limit_location_within_arena( Rect &location ){	

    if( location.x < 0 )
        location.x = 0;

    if( location.y < 0)
        location.y = 0;

    if( location.width + location.x > image.cols )
        location.width = image.cols - location.x;

    if( location.height + location.y > image.rows )
        location.height = image.rows - location.y;
}

int angl( Point2f pt1, Point2f cen, Point2f pt2 ){

    // cross product calculation
    float a = ( float )( ( pt2.x - cen.x ) * ( pt1.y - cen.y ) - ( pt1.x - cen.x ) * ( pt2.y - cen.y ) );
    float b = ( float )( distanc( cen, pt1 ) * distanc( cen, pt2 ) );
    float sin_inv;
    float angle = 1000;

    float dot =(pt2.x - cen.x ) * ( pt1.x - cen.x ) +
        (pt2.y - cen.y) * (pt1.y - cen.y);

    if( b != 0 ){
        sin_inv = asin( a / b ) * 180 / PI;
        
        if( dot < 0 ){
            if ( sin_inv > 0 )
                angle = 180 - sin_inv;
            else
                angle = -180 - sin_inv;
        }
        else if( dot > 0 || dot == 0 && sin_inv != 0 ){
            angle = sin_inv;
        }
        else
            cout << "<" << endl;
    }
    return angle;
    
}
