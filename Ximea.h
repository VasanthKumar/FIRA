#include <xiApi.h>
#include <xiExt.h>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

#define HandleResult(res,place) if (res!=XI_OK) { \
	printf("Error after %s (%d)",place,res); \
	if (xiH) xiCloseDevice(xiH); \
}

class Ximea {
    public:
    HANDLE xiH;
	XI_RETURN stat;
    //image buffer
	XI_IMG image;
    void init_Ximea(int device = 0, int time_us = 10000, int binning_mode = 2, 
    int buffer_queue_size = 3, int buffer_size = 20*1024*1024,
    int recent_frame = 0 );
    Ximea(int device = 0) { xiH = NULL; stat = XI_OK; init_Ximea(device); }
    Mat operator >> (Mat &src_image);
    int get(int param);
};

