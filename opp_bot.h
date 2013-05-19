#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

class opp_bot{
    public:
        static int total_no_of_objects;
        Mat mask;
        Rect location;
        Point center;
        char color;
        void update();
        void FindCenter(Point &cen);
        opp_bot();
        int OBJECT_NUM;
};

extern opp_bot o_bot[5];

