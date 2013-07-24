#pragma once
#include <vector>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;
using namespace std;

RotatedRect largest_contour(Mat image,int &counter);

RotatedRect closest_contour(Mat image,Point closest_to,int flag = 0);

vector <RotatedRect> all_contours(Mat image,vector <int> &,int flag = 0);
