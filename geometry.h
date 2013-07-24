#pragma once
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <stdio.h>
#include "global_var.h"

using namespace cv;
using namespace std;

void threshold( IplImage* src, IplImage* dst );

double distanc( Point2f pt1, Point2f pt2 );

void update_location( Rect &location, Point center );

void expand_location( Rect &location );

void limit_location_within_arena( Rect &location );

int angl( Point2f pt1, Point2f cen, Point2f pt2 );


