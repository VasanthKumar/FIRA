#include "common.h"

extern vector<vector<Point> > contours;
extern vector<Vec4i> hierarchy;

struct ball{
	
	Mat main_image;
	Mat mask;
	Mat tmp;
	
	Point2f center;
	Point2f prev_center;
	Point2f velocity;
	Rect bounding_box;
	
	ball();
	
	void init(Mat src);
	Mat getMask(Mat src, int hue_lower, int hue_upper);	
	void findPosition(int flag);
	void calculate_velocity();
	void display_ball_prop();
	void update();
	
};


