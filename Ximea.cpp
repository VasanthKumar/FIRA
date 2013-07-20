#include "Ximea.h"

void Ximea::init_Ximea(int device, int time_us, int binning_mode, 
    int buffer_queue_size, int buffer_size,
    int recent_frame ) {
    
	image.size = sizeof(XI_IMG);
	image.bp = NULL;
	image.bp_size = 0;

    cout << "Buffer size = " << buffer_size << endl;
	// Retrieving a handle to the camera device
	stat = xiOpenDevice(device, &xiH);
	HandleResult(stat,"xiOpenDevice");

	// Setting "exposure" parameter (10ms)
	stat = xiSetParam(xiH, XI_PRM_EXPOSURE, &time_us, sizeof(time_us), xiTypeInteger);
	HandleResult(stat,"xiSetParam (exposure set)");

	// Change any of the camera's parameter here
	xiSetParamInt(xiH, XI_PRM_IMAGE_DATA_FORMAT, XI_RGB24);
	xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING, binning_mode);
	xiSetParamInt(xiH, XI_PRM_DOWNSAMPLING_TYPE, 1);

	xiSetParamInt(xiH, XI_PRM_RECENT_FRAME, recent_frame);
	xiSetParamInt(xiH, XI_PRM_BUFFERS_QUEUE_SIZE, buffer_queue_size);
	xiSetParamInt(xiH, XI_PRM_ACQ_BUFFER_SIZE, buffer_size);
	
    // Start acquisition
	stat = xiStartAcquisition(xiH);
	HandleResult(stat,"xiStartAcquisition");
	
    stat = xiGetImage(xiH, 1000, &image);
	HandleResult(stat,"xiGetImage");
}

Mat Ximea::operator >> ( Mat & src_image ) {
		stat = xiGetImage(xiH, 1000, &image);
		HandleResult(stat,"xiGetImage");
		src_image.data = (uchar*)image.bp;
        return src_image;
}

int Ximea::get(int param) {
    if(param == 3)
        return image.width;
    if(param == 4)
        return image.height; 
}
