#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>

#include "colors.h"
#include "contours.h"
#include "main.h"
#include "geometry.h"
#include "global_var.h"

using namespace cv;
using namespace std;

class our_bot{

    public:
        char basecolor;
		char lcolor;
		char rcolor;
        Mat* mask;
        Rect location;
        Point front_center, back_center, bot_center;
		Point bot_pos;
		float bot_angle;
		double distance;
		Point dst_point;

        double x,y,angle;

        our_bot();
        void FindCenter();
        double pos();
        double orientation();
        void update();
        bool ball_with_bot_func(Point ball_pos);
        ~our_bot();
};

extern our_bot bot[NUM_OF_OUR_BOTS];
