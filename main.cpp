#include "arena.h"
#include "ball.h"
#include "our_bot.h"
#include "opp_bot.h"
#include <iostream>
#include "wifi.h"
#include "motion.h"
#include <string>
#include "xbee.h"
#include <stdio.h>
#include "Ximea.h"

using namespace cv;
using namespace std;

Mat dst;

Point arena_center;

float integral_error[5] = {0,0,0,0,0};  	//For PID
float prev_error[5] = {0,0,0,0,0}; 
int flag_movt[2] = {0,0};

float velocity = 90;
char charf = 'p';		
bool ball_with_bot;

int main(int argc,char** argv)
{
    Point destinations[10];
    int no_of_dest = 2;
    destinations[0] = Point(110,330);
    destinations[1] = Point(550,260);
    //	destinations[2] = Point(185,265);
    //	destinations[3] = Point(230,230);
    //	destinations[4] = Point(260,330);
    //	destinations[5] = Point(230,370);	
    //	destinations[6] = Point(185,385);
    //	destinations[7] = Point(140,370);	
    int count_of_dest = 0;
    float tol=30;

    Ximea cap(0);
    //VideoCapture cap(argv[1]);

    bool load_file = true;
    if(argc >= 2 && strcmp(argv[1],"f") == 0)
        load_file = false;

    int f=0;

    Mat image = Mat::zeros(cap.get(4),cap.get(3),CV_8UC3);
    cap >> image;
    dst = Mat::zeros(image.rows,image.cols,CV_8UC3);
    Mat warp = Mat::zeros(image.rows,image.cols,CV_8UC3);

    Mat warp_mat;
    Point2f* pts;
    pts = new Point2f [4];

    ball oball = ball();
    arena_center = Point(image.cols/2,image.rows/2);

    int k = 0;

    char c = 0;
    our_bot bot;

    bot.basecolor = 'y';
    bot.lcolor = 'v';
    bot.rcolor = 'g';
    int turn_flag=0;

    double time_for_loop = 0;
    int angle_buffer=0;
    int socket;
    //	int socket = init_wifi(9750,"122.38.0.151");
    char command[4];
    int stop=0;
    int velocity=40;

    //tcpWrite(socket,command);


    //	VideoWriter out("ipfeedback.avi",CV_FOURCC('M','J','P','G'),40,Size(640,480));

    //	int socket = init_wifi(9750,"122.38.0.151");
    //	char command[4]={'0','F','0','0'};
    //	tcpWrite(socket,command);


    //	VideoWriter out("ipfeedback.avi",CV_FOURCC('M','J','P','G'),40,Size(640,480));
    FILE *pFile;
    if(load_file) {
        pFile = fopen("arena_pts.txt", "r");
        for( int m = 0; m < 4; m++ ) {
            fscanf(pFile, "%f %f\n", &pts[m].x, &pts[m].y);
        }
    }
    else
    {
        pts = selectPoint(image);
        pFile = fopen("arena_pts.txt", "w+");
        for( int m = 0; m < 4 ; m++) {
            fprintf(pFile, "%d %d\n", int(pts[m].x), int(pts[m].y));
        } 
        cout << "file writing complete" << endl;
    }
    fclose(pFile);
    cout << "strating warp perspective" << endl;
    warp_mat=getTransformMat(pts);
    cout << "Done getting transform matrix" << endl;
    f=1;
    destroyAllWindows();

    while( c != 27 )
    {
        time_for_loop = (double)cvGetTickCount();

        // getting image from camera
        cap >> image;

        //		resize(image,dst,Size(640,480));
        image.copyTo(dst);

        warp = perspectiveArena(warp_mat,dst);
        //			warp = dst(Rect(pts[0],pts[2]));
        //			warp = dst;
        //			dst.copyTo(warp);
        //			oball.init(warp);

        destinations[0] = oball.center; //for ball

        ball_with_bot = bot.ball_with_bot_func(oball.center);
        cout<<"ball bot : "<<ball_with_bot<<"\t";

        if(count_of_dest == no_of_dest || ball_with_bot == 0)
            count_of_dest = 0;

        bot.dst_point = destinations[count_of_dest];

        cvtColor(warp, dst, CV_BGR2HSV);

        //bot.update();


        cout<<"Distance :"<<bot.distance<<'\t';
        cout<<"Angle :"<<bot.bot_angle<<'\t';
        //		if(bot.bot_angle>0)
        //		{
        if(bot.bot_angle>180){
            bot.bot_angle = bot.bot_angle-360;
        }
        angle_buffer=bot.bot_angle;

        //			cout<<"count : "<<count_of_dest<<"\t";
        //			movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,2,tol);
        //			send_command(command);
        //			if(bot.distance>40){
        //			if((bot.bot_angle>10||bot.bot_angle<-10) && turn_flag==0)
        //			{
        //			turn(0,socket,bot.bot_angle,0,integral_error);
        //			cout<<"turn"<<endl;
        //			}

        cout<<"count  "<<count_of_dest<<"\t";

        //			if(charf == 'p')
        //			{
        //				cin>>charf;
        //				continue;
        //			}
        //			if(charf =='a'){
        //			
        //			if(bot.distance>tol)
        //			{//tol is the tolerance :P
        //				//movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,0,tol,ball_with_bot);
        ////				stop=count_of_dest;
        ////				movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,stop,tol);
        ////			}
        ////            else if(stop==2)
        ////            {
        ////                // Stopping the motors
        ////                tol=0;             // TODO Incorporate tol in the movement code
        ////				movement(0,socket, bot.bot_angle,0,bot.distance,integral_error,flag_movt,prev_error,velocity,stop,tol);
        //			}
        //			
        //			else
        //			{
        ////				command[0] = (char)(48+bot_id);
        ////				command[1] = 's';
        ////       			command[2] = '0';
        ////        		command[3] = 's';
        ////        		send_command(command);
        //				integral_error[0] = 0;
        ////				integral_error[1] = 0;
        ////				integral_error[2] = 0;
        ////				turn_flag=0;
        ////				integral_error[3] = 0;
        ////				integral_error[4] = 0;
        //				if(count_of_dest<no_of_dest)
        //					count_of_dest++;
        //			}
        //			}
        //	
        //
        ////Aby


        //			else
        //				while(1);
        //			turn(0,socket, angle_buffer,90,integral_error);

        //			cout<<(bot.bot_angle)<<'\t';
        //		}



        //		time_for_rect= (double)cvGetTickCount();

        rectangle(warp,Point(bot.bot_center.x-15,bot.bot_center.y-15),Point(bot.bot_center.x+15,bot.bot_center.y+15), Scalar(0,0,255));
        rectangle(warp,oball.bounding_box,Scalar(0,0,255));
        circle(warp,oball.center,3,Scalar(255,255,255),-1);
        for(int i =0; i<no_of_dest;i++){
            circle(warp,destinations[i],3,Scalar(255,255,255),-1);
            circle(warp,destinations[i],tol,Scalar(255,255,255),1);
        }

        circle(warp,Point(destinations[count_of_dest].x,destinations[count_of_dest].y),3,Scalar(0,0,255),-1);
        circle(warp,Point(destinations[count_of_dest].x,destinations[count_of_dest].y),40,Scalar(0,0,255),1);

        //		float D,R,temp_angle;
        //		Point circle_center;
        //		
        //		D = sqrt(pow(bot.bot_center.x-destinations[0].x,2)+pow(bot.bot_center.y-destinations[0].y,2));
        //		cout<<"D : "<<D<<"\n";
        //		R = D/(2*sin(bot.bot_angle*CV_PI/180.0));
        //		cout<<"R : "<<R<<"\n";
        //		if(bot.bot_angle>0)
        //		{
        //			temp_angle = bot.bot_angle - 90;
        //			circle_center.x = bot.bot_center.x - R*sin(temp_angle*CV_PI/180.0);
        //			circle_center.y = bot.bot_center.y - R*cos(temp_angle*CV_PI/180.0);
        //		}
        //		else
        //		{
        //			temp_angle = bot.bot_angle + 90;
        //			circle_center.x = bot.bot_center.x + R*sin(temp_angle*CV_PI/180.0);
        //			circle_center.y = bot.bot_center.y + R*cos(temp_angle*CV_PI/180.0);
        //		}
        //		
        //		circle(warp,circle_center,3,Scalar(255,0,0),-1);


        imshow("warp",warp);

        time_for_loop = ((double)cvGetTickCount() - time_for_loop)/(1000.0*(double)cvGetTickFrequency());

        //		movement(0,socket, bot.bot_angle-180,0,20,integral_error, velocity);

        c = waitKey(1);
        cout<<"\n";

        if(c==27)
            break;
        if(c == ' ')
            c=waitKey(0);
    }
    delete[] pts;
    destroyAllWindows();
    //	cap.release();
    }
