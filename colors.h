#pragma once
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <vector>
#define OUR_JERSEY_COLOR 'y'
#define OPP_JERSEY_COLOR 'b'

using namespace cv;
using namespace std;

void pick_color( Mat &image, Mat &mask, char color );
void pick_basecolor( IplImage * mask, CvRect location, char color );
void pick_frontcolor( IplImage * mask, CvRect location, char color );

