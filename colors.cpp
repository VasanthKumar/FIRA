#include "colors.h"

//inline bool check_orange( int hue, int sat, int value )
//{
//    if((hue>40&&hue<65)&&(sat>50&&sat<=255)&&(value>50&&value<=255))
//        return true;
//    else
//        return false;
//}


//inline bool check_yellow( int hue, int sat, int value )
//{
//    if((hue>27&&hue<35)&&(sat>100&&sat<=255)&&(value>100&&value<=255))
//        return true;
//    else
//        return false;
//}


//inline bool check_blue( int hue, int sat, int value )
//{
//    if((hue>75&&hue<90)&&(sat>100&&sat<=255)&&(value>100&&value<=255))
//        return true;
//    else
//        return false;
//}


//inline bool check_green( int hue, int sat, int value )
//{
//    if((hue>55&&hue<70)&&(sat>100&&sat<=255)&&(value>100&&value<=255))
//        return true;
//    else
//        return false;
//}

//inline bool check_violet( int hue, int sat, int value )
//{
//    if((hue>13&&hue<23)&&(sat>100&&sat<=255)&&(value>100&&value<=255))
//        return true;
//    else
//        return false;
//}

Scalar green_l(55,100,100),green_u(70,180,255);
Scalar blue_l(75,70,100),blue_u(90,255,255);
Scalar violet_l(13,50,120),violet_u(23,220,255);
Scalar yellow_l(27,120,150),yellow_u(35,200,255);
Scalar orange_l(3,100,255),orange_u(13,255,255);


//inline bool check_jersey(int hue,int sat, int value){
//    if(OUR_JERSEY_COLOR == 'y')
//        return check_yellow(hue, sat, value);
//    else
//        return check_blue(hue, sat, value);

//}
//inline bool check_opp_jersey(int hue, int sat, int value){
//    if(OPP_JERSEY_COLOR == 'y')
//        return check_yellow(hue, sat, value);
//    else
//        return check_blue(hue, sat, value);
//}

void pick_color( Mat &image, Mat &mask, char color ){
    int y,x;

    switch( color ){


        case 'g' :
            mask = Scalar(0);

//            for( y = 0; y < image.rows; y++ ){
//                for( x = 0; x < image.cols; x++ ){
//                    if(check_green(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
//                                image.at<Vec3b>(y,x)[2]) )
//                        mask.at<uchar>(y,x) = 255;
//                }
//            }
			inRange(image,green_l,green_u,mask);
            break;

        case 'v' :
            mask = Scalar(0);

//            for( y = 0; y < image.rows; y++ ){
//                for( x = 0; x < image.cols; x++ ){
//                    if(check_violet(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
//                                image.at<Vec3b>(y,x)[2]) )
//                        mask.at<uchar>(y,x) = 255;
//                }
//            }
			inRange(image,violet_l,violet_u,mask);
            break;

        case 'y' :
            mask = Scalar(0);

//            for( y = 0; y < image.rows; y++ ){
//                for( x = 0; x < image.cols; x++ ){
//                    if(check_jersey(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
//                                image.at<Vec3b>(y,x)[2]) )
//                        mask.at<uchar>(y,x) = 255;
//                }
//            }
			inRange(image,yellow_l,yellow_u,mask);
            break;


        case 'o' :
            mask = Scalar(0);

//            for( y = 0; y < image.rows; y++ ){
//                for( x = 0; x < image.cols; x++ ){
//                    if(check_orange(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
//                                image.at<Vec3b>(y,x)[2]) )
//                        mask.at<uchar>(y,x) = 255;
//                }
//            }
			inRange(image,orange_l,orange_u,mask);
            break;

//        case OPP_JERSEY_COLOR :
//            mask = Scalar(0);

//            for( y = 0; y < image.rows; y++ ){
//                for( x = 0; x < image.cols; x++ ){
//                    if(check_opp_jersey(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
//                                image.at<Vec3b>(y,x)[2]) )
//                        mask.at<uchar>(y,x) = 255;
//                }
//            }
//            break;

    }

}
