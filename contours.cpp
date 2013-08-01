#include "contours.h"

inline double distance( CvPoint pt1, CvPoint pt2 ){
    return ( sqrt( ( pt1.x - pt2.x ) * ( pt1.x - pt2.x ) + ( pt1.y - pt2.y ) * ( pt1.y - pt2.y ) ) );
}


RotatedRect largest_contour(Mat img,int &counter)
{
    vector<vector<Point> > contours;

    vector<Vec4i> hierarchy;

    Mat temp = Mat(img.cols,img.rows,CV_8UC1);

    if(img.channels()==3)
    {
        cvtColor(img,temp,CV_BGR2GRAY);
    }
    else if(img.channels()==1)
    {
        img.copyTo(temp);
    }


    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    int i=0,pos=-1,posarea=-1;
    double maxarea=0;
    counter = 0;
    while(i<contours.size())
    {

        if(contourArea(contours[i])>20&&contourArea(contours[i])<600) //constraints on size
        {
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


RotatedRect closest_contour(Mat img,Point closest_to,int flag)
{
    vector< vector<Point> > contours;

    vector<Vec4i> hierarchy;

    Mat temp = Mat(img.cols,img.rows,CV_8UC1);

    if(img.channels()==3)
    {
        cvtColor(img,temp,CV_BGR2GRAY);
    }
    else if(img.channels()==1)
    {
        img.copyTo(temp);
    }

    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    int i=0,counter=0,pos=-1,posarea=-1,closest_index=-1;
    float min_distance = 100000,dis = 0;
    RotatedRect bounding_rect,closest_bounding_rect;

    while(i<contours.size())
    {
        //size constraints should come here...
        if(contourArea(contours[i])>20&&contourArea(contours[i])<600)
        {
            bounding_rect = minAreaRect( contours[i] );
            dis = distance(bounding_rect.center,closest_to);
            if(dis<min_distance)
            {
                closest_bounding_rect = bounding_rect;
                min_distance = dis;
                closest_index = i;
                counter++;
                //cout << "closest area " << contourArea(contours[i]) << endl;
            }
        }	
        i++;

    }
    //cout << "returned" << endl;
    if( counter ){
        return closest_bounding_rect;
    }
    else
        return RotatedRect();
}

vector <RotatedRect> all_contours(Mat img,vector <int> &Area, int flag)
{
    vector<vector<Point> > contours;

    vector<Vec4i> hierarchy;

    Mat temp = Mat(img.cols,img.rows,CV_8UC1);

    if(img.channels()==3)
    {
        cvtColor(img,temp,CV_BGR2GRAY);
    }
    else if(img.channels()==1)
    {
        img.copyTo(temp);
    }
    
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_NONE);

    int i=0,pos=-1,posarea=-1;

    vector <RotatedRect> bounding_rect;

    while(i<contours.size())
    {
        if(contourArea(contours[i])>20&&contourArea(contours[i])<1200) //constraints on size
        {
        cout << "contour area " << contourArea(contours[i]) << endl;
        bounding_rect.push_back( minAreaRect( contours[i] ) );
        Area.push_back(contourArea(contours[i]));
        }	
        i++;
    }
	    
    return bounding_rect;
}
