//#include <boost/thread.hpp> 
#include <cv.h>
#include <highgui.h>

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
		
        double x,y,angle;

        our_bot(); 
        void FindCenter();
        double pos();
        double orientation();
        void update();
        ~our_bot();
};
