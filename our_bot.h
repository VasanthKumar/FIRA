#include <boost/thread.hpp> 
class our_bot{

    public:
        char basecolor;
	char lcolor;
	char rcolor;
        Mat* mask;
        Rect location;
        Point front_center, back_center, bot_center;
        double x,y,angle;

        our_bot(); 
        void FindCenter();
        double pos();
        double orientation();
        void update();
        ~our_bot();
};

extern our_bot bot[5];

