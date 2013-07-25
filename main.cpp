#pragma once
#include <stdio.h>
#include <iostream>
#include <string>

#include "arena.h"
#include "wifi.h"
#include "motion.h"
#include "xbee.h"
#include "global_var.h"
#include "update_frame.h"

using namespace cv;
using namespace std;


float integral_error[5] = {0,0,0,0,0};  	//For PID
float prev_error[5] = {0,0,0,0,0}; 
int flag_movt[2] = {0,0};

float velocity = 90;
char charf = 'p';		
bool ball_with_bot;
int frame = 0;

our_bot bot[NUM_OF_OUR_BOTS];
opp_bot o_bot[NUM_OF_OPP_BOTS];
//ball oball = ball();
opp_bot oball;


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

    bool load_pts_file = true;
    if(argc >= 2 && strcmp(argv[1],"f") == 0)
        load_pts_file = false;

#ifdef Camera
    init(0, load_pts_file);
#else
    init(argv[2], load_pts_file);
#endif

    int f=0;

    //    for(int i = 0; i < NUM_OF_OPP_BOTS; i++)
    //        o_bot[i]();

    int k = 0;

    bot[0].basecolor = 'y';
    bot[0].lcolor = 'g';
    bot[0].rcolor = 'v';

    o_bot[0].color = 'y';
    oball.color = 'o';

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

    int prev_bot_angle = 0;

    //	VideoWriter out("ipfeedback.avi",CV_FOURCC('M','J','P','G'),40,Size(640,480));
    while( c != 27 )
    {
        frame++;

        destinations[0] = oball.center; //for ball

        ball_with_bot = bot[0].ball_with_bot_func(oball.center);
        //cout<<"ball bot[0] : "<<ball_with_bot[0]<<"\t";

        if(count_of_dest == no_of_dest || ball_with_bot == 0)
            count_of_dest = 0;

        bot[0].dst_point = destinations[count_of_dest];

        updateframe();

        circle(image,Point(destinations[count_of_dest].x,destinations[count_of_dest].y),3,Scalar(0,0,255),-1);
        circle(image,Point(destinations[count_of_dest].x,destinations[count_of_dest].y),40,Scalar(0,0,255),1);
        for(int i =0; i<no_of_dest;i++) {
            circle(image,destinations[i],3,Scalar(255,255,255),-1);
            circle(image,destinations[i],tol,Scalar(255,255,255),1);
        }

        display();

        cout << "angle = " << bot[0].bot_angle << '\t';
        cout << "position = " << bot[0].bot_center << endl;
        //if(abs(bot[0].bot_angle - prev_bot_angle) > 50)
        //    waitKey(0);
        prev_bot_angle = bot[0].bot_angle;

        //        cout<<"Distance :"<<bot[0].distance<<'\t';
        //        cout<<"Angle :"<<bot[0].bot[0]_angle<<'\t';
        //		if(bot[0].bot[0]_angle>0)
        //		{
        if(bot[0].bot_angle>180) {
            bot[0].bot_angle = bot[0].bot_angle-360;
        }
        angle_buffer=bot[0].bot_angle;

        //			cout<<"count : "<<count_of_dest<<"\t";
        //			movement(0,socket, bot[0].bot[0]_angle,0,bot[0].distance,integral_error,flag_movt,prev_error,velocity,2,tol);
        //			send_command(command);
        //			if(bot[0].distance>40) {
        //			if((bot[0].bot[0]_angle>10||bot[0].bot[0]_angle<-10) && turn_flag==0)
        //			{
        //			turn(0,socket,bot[0].bot[0]_angle,0,integral_error);
        //			cout<<"turn"<<endl;
        //			}

        cout<<"count  "<<count_of_dest<<"\t";
        cout << endl;
        //			if(charf == 'p')
        //			{
        //				cin>>charf;
        //				continue;
        //			}
        //			if(charf =='a') {
        //			
        //			if(bot[0].distance>tol)
        //			{//tol is the tolerance :P
        //				//movement(0,socket, bot[0].bot[0]_angle,0,bot[0].distance,integral_error,flag_movt,prev_error,velocity,0,tol,ball_with_bot[0]);
        ////				stop=count_of_dest;
        ////				movement(0,socket, bot[0].bot[0]_angle,0,bot[0].distance,integral_error,flag_movt,prev_error,velocity,stop,tol);
        ////			}
        ////            else if(stop==2)
        ////            {
        ////                // Stopping the motors
        ////                tol=0;             // TODO Incorporate tol in the movement code
        ////				movement(0,socket, bot[0].bot[0]_angle,0,bot[0].distance,integral_error,flag_movt,prev_error,velocity,stop,tol);
        //			}
        //			
        //			else
        //			{
        ////				command[0] = (char)(48+bot[0]_id);
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

        //			cout<<(bot[0].bot[0]_angle)<<'\t';
        //		}



        //		time_for_rect= (double)cvGetTickCount();
        //cvtColor(image, image, CV_HSV2BGR);


        //		float D,R,temp_angle;
        //		Point circle_center;
        //		
        //		D = sqrt(pow(bot[0].bot[0]_center.x-destinations[0].x,2)+pow(bot[0].bot[0]_center.y-destinations[0].y,2));
        //		cout<<"D : "<<D<<"\n";
        //		R = D/(2*sin(bot[0].bot[0]_angle*CV_PI/180.0));
        //		cout<<"R : "<<R<<"\n";
        //		if(bot[0].bot[0]_angle>0)
        //		{
        //			temp_angle = bot[0].bot[0]_angle - 90;
        //			circle_center.x = bot[0].bot[0]_center.x - R*sin(temp_angle*CV_PI/180.0);
        //			circle_center.y = bot[0].bot[0]_center.y - R*cos(temp_angle*CV_PI/180.0);
        //		}
        //		else
        //		{
        //			temp_angle = bot[0].bot[0]_angle + 90;
        //			circle_center.x = bot[0].bot[0]_center.x + R*sin(temp_angle*CV_PI/180.0);
        //			circle_center.y = bot[0].bot[0]_center.y + R*cos(temp_angle*CV_PI/180.0);
        //		}
        //		
        //		circle(image,circle_center,3,Scalar(255,0,0),-1);
        //		movement(0,socket, bot[0].bot[0]_angle-180,0,20,integral_error, velocity);

    }
    destroyAllWindows();
    }
