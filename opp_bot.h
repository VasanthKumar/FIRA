#include <cv.h>

class opp_bot{
    public:
        static int total_no_of_objects;
        mat mask;
        Rect location;
        Point center;
        char color;
        void update();
        void FindCenter(Point &cen);
        opp_bot();
        int OBJECT_NUM;
};

extern opp_bot o_bot[5];

