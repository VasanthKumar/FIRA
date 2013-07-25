#include "ball.h"

#define WIDTH 1280
#define HEIGHT 1024
#define DEPTH 3

vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

Mat ball::getMask(Mat src,int hue_lower,int hue_upper)
{
    //printf("\nentering getMask\n");
    double time_for_mask = (double)cvGetTickCount();

    Mat temp_image = Mat::zeros(src.cols,src.rows,CV_8UC3);
    Mat temp_image_gray = Mat::zeros(src.cols,src.rows,CV_8UC1);
    //Mat element = getStructuringElement(MORPH_RECT,Size( 2*2 + 1, 2*2+1 ),Point( -1,-1) );

    cvtColor(src,temp_image,CV_BGR2HSV);

    inRange(temp_image,Scalar(hue_lower,100,100),Scalar(hue_upper,255,255),temp_image_gray);
    time_for_mask = ((double)cvGetTickCount() - time_for_mask)/(1000.0*(double)cvGetTickFrequency());

    //printf("\nTime for getting mask : %lf\n",(double)time_for_mask);
    //morphologyEx(temp_image_gray,temp_image_gray,MORPH_CLOSE,element);

//    imshow("temp gray",temp_image_gray);
    //printf("\nLeaving getMask\n");
    return temp_image_gray;
}

void ball::findPosition(int flag=0)
{
    //printf("\nEntering findPosition\n");

    double time_for_find = (double)cvGetTickCount();
    Mat temp = Mat(mask.cols,mask.rows,CV_8UC1);
    Mat display = Mat::zeros(Size(WIDTH,HEIGHT),DEPTH);

    mask.copyTo(temp);

    //imshow("temp",temp);

    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE,Point2f(bounding_box.x,bounding_box.y));

    int i=0,counter=0,pos=-1,posarea=-1;
    vector<Rect>boundrect(contours.size());
    double maxarea=0;


    while(i<contours.size())
    {

        if(contourArea(contours[i])>10&&contourArea(contours[i])<600&&hierarchy[i][3]<0) //constraints on size
        {
            boundrect[i] = boundingRect(contours[i]);
            rectangle(display,boundrect[i].tl(),boundrect[i].br(),Scalar(255,0,0));
            counter++;
            pos = i;
            if(contourArea(contours[i])>maxarea)
            {
                maxarea = contourArea(contours[i]);
                posarea = i;

            }
        }	
        i++;
    }

//cout<<"counter = "<<counter<<endl;

    if(counter)
    {

        bounding_box.width = boundrect[posarea].width*2;
        bounding_box.height = boundrect[posarea].height*2;

        prev_center = center;
        center.x = boundrect[posarea].x + boundrect[posarea].width/2;
        center.y = boundrect[posarea].y + boundrect[posarea].height/2; 
        calculate_velocity();

        bounding_box.x = boundrect[posarea].x - boundrect[posarea].width/2 + velocity.x;
        bounding_box.y = boundrect[posarea].y - boundrect[posarea].height/2 + velocity.y;

        // bounding_box.width+= fabs(velocity.x);
        // bounding_box.height+= fabs(velocity.y);

        if(bounding_box.x<0)
        {
            bounding_box.x=0;
        }
        if(bounding_box.y<0)
        {
            bounding_box.y=0;
        }
        if(bounding_box.x>WIDTH)
        {
            bounding_box.x=0;
            bounding_box.width=WIDTH;
        }
        if(bounding_box.y>HEIGHT)
        {
            bounding_box.y=0;
            bounding_box.height=HEIGHT;
        }

        if(bounding_box.x+bounding_box.width>WIDTH)
        {
            bounding_box.x=WIDTH-bounding_box.width;
        }
        if(bounding_box.y+bounding_box.height>HEIGHT)
        {
            bounding_box.y=HEIGHT-bounding_box.height;
        }


    }
    else
    {
        bounding_box.x=0;
        bounding_box.y=0;
        bounding_box.width=WIDTH;
        bounding_box.height=HEIGHT;
        prev_center.x = prev_center.x + velocity.x;
        prev_center.y = prev_center.y + velocity.y; 

    }
    //imshow("display",display);
    time_for_find = ((double)cvGetTickCount() - time_for_find)/(1000.0*(double)cvGetTickFrequency());
    //printf("\nTime for getting find : %lf\n",(double)time_for_find);


}
void ball::calculate_velocity()
{
    velocity.x = center.x - prev_center.x;
    velocity.y = center.y - prev_center.y;
}

ball::ball()
{
    main_image = Mat(WIDTH,HEIGHT,CV_8U);

    center = Point2f(0.0,0.0);
    prev_center = Point2f(0.0,0.0);
    calculate_velocity();
    bounding_box = Rect(0,0,WIDTH,HEIGHT);

}

void ball::init(Mat src)
{
    //printf("\nEntering init\n");
    //src.copyTo(main_image);

    tmp = src(bounding_box);
    mask= getMask(tmp,4,13);
    //waitKey(0);

    //mask = temp(bounding_box);

    findPosition();
    //tmp.release();
    //printf("\nLeaving init\n");
}

void ball::display_ball_prop()
{
    printf("\nCenter:(%f,%f)\t",center.x,center.y);
    printf("\nPrevCentre:(%f,%f)\n",prev_center.x,prev_center.y);
    printf("\nVelocity:(%f,%f)\n",velocity.x,velocity.y);
    printf("\nBBOX:(%d,%d,%d,%d)\n",bounding_box.x,bounding_box.y,bounding_box.width,bounding_box.height);

}
void ball::update()
{

}
