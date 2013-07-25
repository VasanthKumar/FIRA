#pragma once
#include <cv.h>
#include <highgui.h>

#include "global_var.h"
#include "colors.h"
#include "contours.h"
#include "geometry.h"

using namespace  cv;
using namespace std;

class opp_bot{
    public:
        static int total_no_of_objects;
        Mat mask;
        Rect location;
        Point center;
        char color;
        void update();
        void FindCenter();
        opp_bot();
        int OBJECT_NUM;
};

extern opp_bot o_bot[NUM_OF_OPP_BOTS];
extern opp_bot oball;
