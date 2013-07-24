#pragma once
#include <boost/thread/thread.hpp>
#include <highgui.h>
#include <cv.h>
#include <math.h>
#include <SerialStream.h>
#include "geometry.h"
#include "our_bot.h"
#include "opp_bot.h"
#include "global_var.h"
#include "ball.h"
#include "arena.h"
#include "main.h"

using namespace cv;
using namespace std; 

void init_image(bool load_pts_file);

template <class Device>
void init(Device arg, bool load_pts_file) {
    cap.open(arg);
    init_image(load_pts_file);
}

void updateframe();

void display();
