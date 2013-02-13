#include "contours.h"

inline double distance( CvPoint pt1, CvPoint pt2 ){
    return ( sqrt( ( pt1.x - pt2.x ) * ( pt1.x - pt2.x ) + ( pt1.y - pt2.y ) * ( pt1.y - pt2.y ) ) );
}


RotatedRect largest_contour(Mat image,int &counter)
{
    vector<vector<Point> > contours;

    vector<Vec4i> hierarchy;

    Mat temp = Mat(image.cols,image.rows,CV_8UC1);

    Mat display = Mat::zeros(temp.cols,temp.rows,CV_8UC3);

    if(image.channels()==3)
    {
        cvtColor(image,temp,CV_BGR2GRAY);
    }
    else if(image.channels()==1)
    {
        image.copyTo(temp);
    }


    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    int i=0,pos=-1,posarea=-1;
    double maxarea=0;
    counter = 0;
    while(i<contours.size())
    {

        if(contourArea(contours[i])>50&&contourArea(contours[i])<600) //constraints on size
        {
            //rectangle(display,boundrect[i].tl(),boundrect[i].br(),Scalar(255,0,0));
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

    RotatedRect bounding_rect;
    if( counter ){
        bounding_rect = minAreaRect( contours[posarea] );
        return bounding_rect;
    }
    else
        return RotatedRect();
}


RotatedRect closest_contour(Mat image,Point closest_to,int flag=0)
{
    vector<vector<Point> > contours;

    vector<Vec4i> hierarchy;

    Mat temp = Mat(image.cols,image.rows,CV_8UC1);

    Mat display = Mat::zeros(temp.cols,temp.rows,CV_8UC3);


    if(image.channels()==3)
    {
        cvtColor(image,temp,CV_BGR2GRAY);
    }
    else if(image.channels()==1)
    {
        image.copyTo(temp);
    }

    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    int i=0,counter=0,pos=-1,posarea=-1,closest_index=-1;
    float min_distance = 100000,dis = 0;
    RotatedRect bounding_rect,closest_bounding_rect;

    while(i<contours.size())
    {
        //size constraints should come here...
        if(contourArea(contours[i])>50&&contourArea(contours[i])<600)
        {
            bounding_rect = minAreaRect( contours[i] );
            dis = distance(bounding_rect.center,closest_to);
            if(dis<min_distance)
            {
                closest_bounding_rect = bounding_rect;
                min_distance = dis;
                closest_index = i;
            }
        }	
        i++;

    }

    if( counter ){
        return closest_bounding_rect;
    }
    else
        return RotatedRect();
}


vector <RotatedRect> all_contours(Mat image, int flag=0)
{
    vector<vector<Point> > contours;

    vector<Vec4i> hierarchy;

    Mat temp = Mat(image.cols,image.rows,CV_8UC1);

    Mat display = Mat::zeros(temp.cols,temp.rows,CV_8UC3);


    if(image.channels()==3)
    {
        cvtColor(image,temp,CV_BGR2GRAY);
    }
    else if(image.channels()==1)
    {
        image.copyTo(temp);
    }

    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    int i=0,counter=0,pos=-1,posarea=-1;
    double maxarea=0;

    while(i<contours.size())
    {

        if(contourArea(contours[i])>50&&contourArea(contours[i])<600) //constraints on size
        {
            //rectangle(display,boundrect[i].tl(),boundrect[i].br(),Scalar(255,0,0));
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

    vector <RotatedRect> bounding_rect;
    while( counter ){
        bounding_rect.push_back( minAreaRect( contours[counter - 1] ) );
        counter--;
    }
    return bounding_rect;
}
