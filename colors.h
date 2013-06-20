#pragma once
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <vector>
#include "common.h"
#define OUR_JERSEY_COLOR 'b'
#define OPP_JERSEY_COLOR 'y'

using namespace cv;
using namespace std;

void pick_color( Mat &image, Mat &mask, char color );
void pick_basecolor( Mat mask, CvRect location, char color );
void pick_frontcolor( Mat mask, CvRect location, char color );

