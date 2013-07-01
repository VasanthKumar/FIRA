/*
**We shall use the direction of scoring as zero angle. And angle varies from -180 to 180
*/

#include"wifi.h"
#include "motion.h"
#include<iostream>

using namespace std;

/*
**The shoot command is to be used when there is no path for the bot to go but there is space for the ball to go.
**The function rotates the bot at a very high speed in the required direction.
**It takes in the following arguements. 
**bot_id: The bot to which the command needs to be sent.
**angle:  This angle is the angle betweent the bot and the goal.	
**socket: The socket id of the wifi module on the bot
*/

void shoot(float angle,int bot_id,int socket)  
{
	char command[4];
	if(angle>45 && angle<135)			//rotate to the right
	{
		command[0] = (char)(48+bot_id);
		command[1] = 'R';
		command[2] = '~';
		command[3] = '~';	
	}
	else if(angle>225 && angle<315)		//rotate to the left
	{
		command[0] = (char)(48+bot_id);
		command[1] = 'L';
		command[2] = '~';
		command[3] = '~';
	}
	tcpWrite(socket,command);
}

/*
**The pass command will be used to when we need to pass the ball to another bot
**The function increases the speed to its maximum so that it hits the ball in front.
**the bot takes in the following arguements
**bot_id: The bot to which the command needs to be sent
**socket: The socket id of the wifi module on the bot
*/
void pass(int bot_id,int socket)
{
	char command[4];
	command[0] = (char)(48+bot_id);
	command[1] = 'F';
	command[2] = '~';
	command[3] = '~';	
	tcpWrite(socket,command);
}

/*
**This is the motion command. It basically handles movement of bots using PID.
**It takes the following arguements
**bot_id: The bot to which the command needs to be sent
**socket: The socket id of the wifi module on the bot
**bot_angle: Orientation of the bot
**angle: Direction it is actually supposed to go
**dist: Distance to destination
**integral_error: integral error for PID
**velocity: maximum velocity of travel
*/
void movement(int bot_id, int socket, float bot_angle,float angle, float dist, float integral_error[5]/*,int velocity */)
{
	int velocity=50; // shud be deleted
	int left, right; 						//Values to the wheels
	float error, difference, integral, derivative,time = 0.014;
	float kp = 0.45, ki = 1.2;
	char command[4];
	command[0] = (char)(48+bot_id);

	if(dist<10)
	{
			command[1] = 's';
			command[2] = '!';
			command[3] = '!';
	}
	else
	{
		difference = bot_angle - angle;
	
		if(difference<=90 && difference>=-90)
		{
			integral_error[bot_id] = integral_error[bot_id] + difference*time;
			//derivative = (difference - prev_error[bot_id])/time; 
			error = (kp*difference);// + (ki*integral_error[bot_id])+ (kd*derivative);

			right = ((30 - error)*velocity/60.0);
			left = ((error + 30)*velocity/60.0);
		
			if(right>velocity)
				right = velocity;
			if(left>velocity)
				left = velocity;
			
			if(right<-velocity)
				right =	-velocity;
			if(left<-velocity)
				left = -velocity;
		
			if(right>0 && left>0)
			{
				command[1] = 'F';
				command[2] = (char)right;
				command[3] = (char)left;
			}
			else if(right>0 && left<0)
			{
				command[1] = 'R';
				command[2] = (char)right;
				command[3] = (char)(-1*left);
			}
			else if(right<0 && left>0)
			{
				command[1] = 'L';
				command[2] = (char)(-1*right);
				command[3] = (char)(left);
			}
		}

		else
		{
			if(difference>90)
				difference = 180 - difference;
			else if(difference<-90)
				difference = -180-difference;
			
			integral_error[bot_id] = integral_error[bot_id] + difference*time;
			//derivative = (difference - prev_error[bot_id])/time; 
			error = (kp*difference);// + (ki*integral_error[bot_id])+ (kd*derivative);

			left = ((30 - error)*velocity/60.0);
			right = ((error + 30)*velocity/60.0);
		
			if(right>velocity)
				right = velocity;
			if(left>velocity)
				left = velocity;
			
			if(right<-velocity)
				right =	-velocity;
			if(left<-velocity)
				left = -velocity;
		
			if(right>0 && left>0)
			{
				command[1] = 'B';
				command[2] = (char)right;
				command[3] = (char)left;
			}
			else if(right>0 && left<0)
			{
				command[1] = 'L';
				command[2] = (char)right;
				command[3] = (char)(-1*left);
			}
			else if(right<0 && left>0)
			{
				command[1] = 'R';
				command[2] = (char)(-1*right);
				command[3] = (char)(left);
			}
		}

		tcpWrite(socket,command);
//		cout<<command<<'\n';	
	}
}
			
	
