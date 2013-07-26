#include "our_bot.h"

//our_bot::dst_point = cv::Point(640,0);

bool our_bot::ball_with_bot_func(Point ball_pos)
{
	double dist = distanc(ball_pos,back_center);
//	cout<<"dist :"<<dist<<"\t";
//	cout<<"  "<<ball_pos.x<<"  "<<bot_pos.x<<"\t";
	if(dist<23)
		return true;
	else
		return false;
}

our_bot::our_bot(){
    x = 0, y = 0, angle = 0;
    mask = new Mat[3];
    mask[0] = Mat( Size(image.cols,image.rows), CV_8UC1 );
    mask[1] = Mat( Size(image.cols,image.rows), CV_8UC1 );
    mask[2] = Mat( Size(image.cols,image.rows), CV_8UC1 );
}

our_bot::~our_bot(){
    mask[0].release();
    mask[1].release();
    mask[2].release();
    delete mask;
}

Rect goal_rect = Rect(0,480*260/180,640,480*200/180);

double our_bot::pos(){
    Point center;
    center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );

    // making arena center as x=0 and y=0
    x = ( ( center.x - arena_center.x ) *250 ) / goal_rect.width;
    y=-1*((center.y-arena_center.y)*250)/goal_rect.width;
    
	bot_pos.x = x;
	bot_pos.y = y;
//	printf("botx=%f boty=%f\n",x,y);

}

double our_bot::orientation(){

    angle = angl( Point( dst_point.x,dst_point.y ), back_center, front_center );

//    if( angle < 0 )
//        angle = -1 * angle;
//    else if( angle == 1000)		//case where the bot is not found in the bounding box.
//        angle = 0;
//    else if( angle > 0 )
//        angle = 360 - angle;

	bot_angle = angle;
//	cout<<"Distance: "<<sqrt(pow(back_center.x-dst_point.x,2)+pow(back_center.y-dst_point.y,2))<<"\t";
	distance = sqrt(pow(back_center.x-dst_point.x,2)+pow(back_center.y-dst_point.y,2));

//    printf("angle%f\t",angle);

}


void our_bot::FindCenter(){

    vector <int> Area_base;
    int Area_frontl=0;
    int Area_frontr=0;	
    vector <RotatedRect> BaseCenter = all_contours( mask[0], Area_base,0 );

    Point frontl_center = Point( 0, 0 );
    Point frontr_center = Point( 0, 0 );
    RotatedRect front_left;

    RotatedRect front_right;
    back_center = Point( 0, 0 );
	
//	cout<<"BaseCenter size : "<<BaseCenter.size()<<"\n";
    for( int i = 0; i < BaseCenter.size(); i++ ){

//		cout<<"before contour if else\n";
        if(Area_base[i] > BASE_AREA_THRESH) {
            front_left = closest_contour( mask[1], BaseCenter[i].center, Area_frontl );
            front_right = closest_contour( mask[2], BaseCenter[i].center, Area_frontr );
//			cout<<front_left.center.x<<"  "<<front_right.center.x<<"\n";
			
            if( front_left.center.x != 0 && front_right.center.x !=0 ){

                float angle_check =angl(front_right.center,BaseCenter[i].center,front_left.center); 
                cout << "angle check " << angle_check << endl;
                if( //angle_check > 40 && angle_check < 80  &&
                        distanc(front_left.center,BaseCenter[i].center) < BOT_LENGTH && 
                        distanc(front_right.center,BaseCenter[i].center) < BOT_LENGTH )
                {
//                	cout<<"finding centers\n";
                    back_center = BaseCenter[i].center;
                    frontl_center = front_left.center;
                    frontr_center = front_right.center;

                    break;
                }	

            }
        }
    }
    front_center = Point( ( frontl_center.x + frontr_center.x ) / 2, ( frontl_center.y + frontr_center.y ) / 2 );
}

void our_bot::update(){

    Mat roi_image;
    roi_image = image(location);

    GaussianBlur( roi_image, roi_image, Size( 3, 3 ), 0, 0 );
    
    cvtColor(roi_image, roi_image, CV_BGR2HSV);
    
    mask[0] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);
    mask[1] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);
    mask[2] = Mat::zeros(roi_image.rows,roi_image.cols,CV_8UC1);

    pick_color( roi_image,mask[0], basecolor );
    pick_color( roi_image,mask[1], lcolor );
    pick_color( roi_image,mask[2], rcolor );
   
	imshow("base",mask[0]);
	imshow("left",mask[1]);
	imshow("right",mask[2]);
    
    FindCenter();
    
   cvtColor(roi_image, roi_image, CV_HSV2BGR);

    if( front_center.x != 0 && back_center.x != 0 ){
        //Taking care for the relative position changes due to ROI.
        front_center = Point( front_center.x + location.x, front_center.y + location.y );
        back_center = Point( back_center.x + location.x, back_center.y + location.y );
        bot_center = Point( ( front_center.x + back_center.x ) / 2, ( front_center.y + back_center.y ) / 2 );
        update_location( location, bot_center );
        //        cout<<"bot found!!"<<endl;
    }
    else{
        expand_location( location );
    }

    limit_location_within_arena( location );

    //    cout<<"location = "<<location.x<<' '<<location.y
    //        <<' '<<location.width<<' '<<location.height<<endl;
    pos();
    orientation();
}


