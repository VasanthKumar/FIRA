#include "update_frame.h"

#ifdef Camera
    Ximea cap;
#else
    VideoCapture cap;
#endif

Mat image;
Point arena_center;
char c;
Mat image_mat;

void init_image( bool load_pts_file) {
    image = Mat::zeros(cap.get(4),cap.get(3),CV_8UC3);
    arena_center = Point(image.cols/2,image.rows/2);
    for(int i=0; i < NUM_OF_OUR_BOTS; i++) {
        bot[i].location = Rect(0, 0, image.cols, image.rows);
    }
    for(int i=0; i < NUM_OF_OPP_BOTS; i++) {
        o_bot[i].location = Rect(0, 0, image.cols, image.rows);
    }
        oball.location = Rect(0, 0, image.cols, image.rows);
 

    Point2f* pts;
    pts = new Point2f [4];

    FILE *pFile;
    if(load_pts_file) {
        pFile = fopen("arena_pts.txt", "r");
        for( int m = 0; m < 4; m++ ) {
            fscanf(pFile, "%f %f\n", &pts[m].x, &pts[m].y);
        }
    }
    else
    {
        cap >> image;
        pts = selectPoint(image);
        pFile = fopen("arena_pts.txt", "w+");
        for( int m = 0; m < 4 ; m++) {
            fprintf(pFile, "%d %d\n", int(pts[m].x), int(pts[m].y));
        } 
        cout << "file writing complete" << endl;
    }

    fclose(pFile);
    cout << "starting image perspective" << endl;
    image_mat=getTransformMat(pts);
    cout << "Done getting transform matrix" << endl;
    destroyAllWindows();
    
    namedWindow("image", CV_WINDOW_NORMAL);
    resizeWindow("image", 500, 480);
}

void update_our_bot()
{
    for( int i=0;i < NUM_OF_OUR_BOTS;i++)
        bot[i].update();
}

void update_opp_bot()
{
    for( int i=0;i < NUM_OF_OPP_BOTS;i++)
        o_bot[i].update();
}

void updateframe(){
    double time_for_loop = (double)cvGetTickCount();

    // getting image from camera
    cap >> image;
    //image = perspectiveArena(image_mat,image);

   // cout << "cam image = " << ((double)cvGetTickCount() -
   //         time_for_loop)/(1000.0*(double)cvGetTickFrequency()) << '\t';

    double time_bot_update = (double)cvGetTickCount();
    
    update_our_bot();
    update_opp_bot();

    time_bot_update = ((double)cvGetTickCount() - time_bot_update)/(1000.0*(double)cvGetTickFrequency());
    //cout << "bot update = " << time_bot_update << '\t';

    double ball_detection_time = (double)cvGetTickCount();
    oball.update();
    //cout << "ball detection time image = " << ((double)cvGetTickCount() -
    //       ball_detection_time)/(1000.0*(double)cvGetTickFrequency()) << '\t';
}

void display() {

    circle( image, arena_center, 10, CV_RGB( 180, 180, 255 ), -1, 8, 0 );

    for( int i = 0; i < NUM_OF_OUR_BOTS; i++ ){
        circle( image, bot[i].front_center, 5, CV_RGB( 255, 0, 0 ), -1, 8, 0 );
        circle( image, bot[i].back_center, 2, CV_RGB( 255, 255, 255 ), -1, 8, 0 );
        line( image, bot[i].front_center, bot[i].back_center,
                CV_RGB( 255, 255, 255 ), 2, 8, 0);
        rectangle( image, Point( bot[i].location.x, bot[i].location.y ),
                Point( bot[i].location.x + bot[i].location.width, bot[i].location.y + bot[i].location.height ),
                Scalar( 255, 0, 0, 0 ), 1, 4, 0 );
    }

    for( int i = 0; i < NUM_OF_OPP_BOTS; i++ ){
        circle( image, o_bot[i].center, 5, CV_RGB( 255, 255, 255 ), -1, 8, 0 );
        rectangle( image, Point( o_bot[i].location.x, o_bot[i].location.y ),
                Point( o_bot[i].location.x + o_bot[i].location.width, o_bot[i].location.y + o_bot[i].location.height ),
                Scalar( 0, 0, 255, 0 ), 1, 4, 0 );
    }

    imshow("image",image);
    
    c = waitKey(1);
    if(c == ' ')
        c=waitKey(0);
}
