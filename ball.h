#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>


#define Width 800
#define Height 600
#define Depth CV_8UC3
using namespace cv;

extern vector<vector<Point> > contours;

extern vector<Vec4i> hierarchy;
extern bool drawing_box;
extern Rect box;
extern Mat image;
extern int flag;


struct ball{
	
	Mat main_image;
	Mat ball_image;
	Point2f pos_ball_centre;
	Point2f pos_ball_centre_prev;
	Point2f vel_ball;
	Rect dbox;
	Rect bbox;
	
	ball();
	
	void init(Mat src);
	Mat hueRange (Mat src, int hue_lower, int hue_upper, int flag=0);
	void con(int flag=0);
	void calc_velocity();
	void disp_ball_prop();
	void update();
	
};
void draw_box( Mat img, Rect rect );
double dist( Point2f a,Point2f b );
static void onMouse( int event, int x, int y, int, void* );
Rect Roi( Mat argm );

