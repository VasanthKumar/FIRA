#include "colors.h"

inline bool check_orange( int hue, int sat, int value )
{
    if((hue>40&&hue<65)&&(sat>50&&sat<=230)&&(value>50&&value<=230))
        return true;
    else
        return false;
}


inline bool check_yellow( int hue, int sat, int value )
{
    if((hue>25&&hue<35)&&(sat>90&&sat<=255)&&(value>90&&value<=255))
        return true;
    else
        return false;
}


inline bool check_blue( int hue, int sat, int value )
{
    if((hue>75&&hue<90)&&(sat>50&&sat<=250)&&(value>50&&value<=250))
        return true;
    else
        return false;
}


inline bool check_green( int hue, int sat, int value )
{
    if((hue>55&&hue<70)&&(sat>50&&sat<=250)&&(value>50&&value<=250))
        return true;
    else
        return false;
}

inline bool check_violet( int hue, int sat, int value )
{
    if((hue>40&&hue<65)&&(sat>50&&sat<=250)&&(value>50&&value<=250))
        return true;
    else
        return false;
}

inline bool check_jersey(int hue,int sat, int value){
    if(OUR_JERSEY_COLOR == 'y')
        return check_yellow(hue, sat, value);
    else
        return check_blue(hue, sat, value);
}

inline bool check_opp_jersey(int hue, int sat, int value){
    if(OPP_JERSEY_COLOR == 'y')
        return check_yellow(hue, sat, value);
    else
        return check_blue(hue, sat, value);
}

void pick_color( Mat &image, Mat &mask, char color ){
    int y,x;

    switch( color ){


        case 'g' :
            mask = Scalar(0);

            for( y = 0; y < image.rows; y++ ){
                for( x = 0; x < image.cols; x++ ){
                    if(check_green(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
                                image.at<Vec3b>(y,x)[2]) )
                        mask.at<uchar>(y,x) = 255;
                }
            }
            break;

        case 'v' :
            mask = Scalar(0);

            for( y = 0; y < image.rows; y++ ){
                for( x = 0; x < image.cols; x++ ){
                    if(check_violet(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
                                image.at<Vec3b>(y,x)[2]) )
                        mask.at<uchar>(y,x) = 255;
                }
            }
            break;

        case OUR_JERSEY_COLOR :
            mask = Scalar(0);

            for( y = 0; y < image.rows; y++ ){
                for( x = 0; x < image.cols; x++ ){
                    if(check_jersey(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
                                image.at<Vec3b>(y,x)[2]) )
                        mask.at<uchar>(y,x) = 255;
                }
            }
            break;


        case 'o' :
            mask = Scalar(0);

            for( y = 0; y < image.rows; y++ ){
                for( x = 0; x < image.cols; x++ ){
                    if(check_orange(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
                                image.at<Vec3b>(y,x)[2]) )
                        mask.at<uchar>(y,x) = 255;
                }
            }
            break;

        case OPP_JERSEY_COLOR :
            mask = Scalar(0);

            for( y = 0; y < image.rows; y++ ){
                for( x = 0; x < image.cols; x++ ){
                    if(check_opp_jersey(image.at<Vec3b>(y,x)[0],image.at<Vec3b>(y,x)[1],
                                image.at<Vec3b>(y,x)[2]) )
                        mask.at<uchar>(y,x) = 255;
                }
            }
            break;

    }

}
