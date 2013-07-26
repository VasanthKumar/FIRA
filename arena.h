#pragma once
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "global_var.h"

using namespace cv;
using namespace std;

void draw_box( Mat img, Rect rect );
void draw_circle( Mat img, Point2f center, int radius );
static void onMouseRect( int event, int x, int y, int, void* );
Rect selectRect( Mat argm );
static void onMouseSelect( int event, int x, int y, int, void* );
Point2f* selectPoint(Mat src);
Mat getTransformMat(Point2f* srcq );
Mat perspectiveArena(Mat warp_Mat,Mat src);

