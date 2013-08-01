#pragma once
#include <cv.h>
#include <highgui.h>

#include "global_var.h"
#include "colors.h"
#include "main.h"
#include "contours.h"
#include "geometry.h"

using namespace  cv;
using namespace std;

class ball{
    public:
        Mat *mask;
        Rect location;
        Point velocity;
        Point prev_velocity;
        Point center;
        Point prev_center;
        char color;
        void update();
        void FindCenter();
        void calculate_velocity();
        ball();
};

extern ball oball;
