/*
**We shall use the direction of scoring as zero angle. And angle varies from -180 to 180
*/

#include"wifi.h"
#include "motion.h"
#include<iostream>
#include "xbee.h"
#include<queue>
#include<math.h>
#include<iomanip>

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

bool stopped(int bot_id, int socket, int curr[2], int prev[2], float tol){
	
	float temp;
	queue<float> dist;
	temp = dist.back();
	float bot_distance;
	bot_distance = sqrt(pow(prev[0]-curr[0],2)+pow(prev[1]-curr[1],2));
	if(dist.size()<5 || dist.empty()){
		dist.push(bot_distance);
		if((bot_distance - temp) > tol){
			for(int i = 0; i< dist.size(); i++)
				dist.pop();
		}		
		prev[0] = curr[0];	
		prev[1] = curr[1];
		return false;	
	}			
	else
		return true;
	
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

int movement(int bot_id, int socket, float bot_angle,float angle, float dist, float integral_error[5],int flag[2],float prev_error[5],int velocity,int stop,float tol,bool ball_with_bot)
{
    	//flag variables
    float m=1;                  //shud be deleted
	int left, right, direction = 1,decel=1; 					//Values to the wheels
	float correction, difference,derivative;            //integral, derivative, time = 0.014;
	float kp, ki, kd;
	if(ball_with_bot){
		kp = 0.7;
		ki = 0;
		kd = 0.06;
	}
//	kp = 0.4;
	else{
		kp = 0.4;
		ki = 0;
		kd = 0.07;
	}
	char command[4];
    char *bot1_buffer = (char *)malloc(BUFFER_SIZE*sizeof(char));
    #define BUFFER_SIZE 8
	command[0] = (char)(48+bot_id);
    flag[0] = flag[0] + 1;										// TODO Flag[0] is to remove the frame error @ the beginning of the code.
    															// Flag[1] is for the acceleration to build up slowly
	
    if(flag[0]>20)
    {
			flag[1] += 1;
			difference = bot_angle;// - angle;
			if(difference>90)		// Second Quadrant(Back motion)
			{
				direction = -1;
				difference= 180 - (difference);
			}
			else if(difference<-90)	// Second Quadrant(Back motion)
			{
				direction = -1;
				difference = abs(difference) - 180;
			}
			
			derivative = abs(difference) - abs(prev_error[bot_id]);
			correction = (kp*difference) + (ki*integral_error[0])+ (kd*derivative);
			prev_error[bot_id] = difference;						
			integral_error[bot_id] += correction;								
            cout<<std::setprecision(3)<<"correction:"<<correction<<"\t";
			
//			switch(stop)
//			{
//				case 1:{	// Decelerate to a velocity of 30	
//							m=(dist-tol)/30;					// Bringing down the velocity break;
//				       }
//				case 2:{	// Decelerate to a complete stop
//							velocity=30;					// You can't stop without going to 30 speed first
//							if(dist>45)
//								m=1;
//							else if(dist>25 && dist<=45)
//								m=(dist-20)/25;
//							else if(dist>25 && dist<=45)
//								m=(dist-20)/25;	
//							else
//								m=0;
//							if(!stopped()){
//								command[1] = 's';
//								command[2] = '0';
//								command[3] = 's';
//								send_command(command);
//								return 0;
//							}	
//                            break;
//						}
//				default:{	// No deceleration
//							m=1;
//                            break;
//						}
//				cout<<" Stop is "<<stop<<" and m is "<<m<<"\t";
//			}
			if(difference>0)
			{
    			left = direction*velocity*m;				
				right = direction*(velocity - abs(correction))*m;
				if(right<(-1*velocity))
					right = -1*velocity;
			}
			if(difference<0) 
			{
    			right = direction*(velocity)*m;				
    			left = direction*(velocity - abs(correction))*m;
				if(left<(-1*velocity))
					left = -1*velocity;
			}
		    if(flag[1]<10)
		    {
			    left=(left/10)*flag[1];
			    right=(right/10)*flag[1];
		    }
			cout<<"Left: "<<left<<"\tRight: "<<right<<"\t";
			if(direction >0){
				if(right>0 && left>0)
				{
					command[1] = 'F';
					command[2] = (char)right;
					command[3] = (char)(left);
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
				else if(right<0 && left<0)
				{
					command[1] = 'B';
					command[2] = (char)abs(right);
					command[3] = (char)abs(left);
				}
			}
			else if(direction<0){
					if(right>0 && left>0)
				{
					command[1] = 'F';
					command[2] = (char)left;
					command[3] = (char)(right);
				}
				else if(right>0 && left<0)
				{
					command[1] = 'R';
					command[2] = (char)abs(left);
					command[3] = (char)abs(right);
				}
				else if(right<0 && left>0)
				{
					command[1] = 'L';
					command[2] = (char)abs(left);
					command[3] = (char)abs(right);
				}
				else if(right<0 && left<0)
				{
					command[1] = 'B';
					command[2] = (char)abs(left);
					command[3] = (char)abs(right);
				}
				
			}

		}
		
    
    else
    {
        command[1] = 's';
        command[2] = '0';
        command[3] = 's';
    }

		tcpWrite(socket,command);
//		send_command(command);
		return 0;
// printf("Got : %s \n", tcpRead(socket,bot1_buffer));
}


void turn(int bot_id, int socket, float bot_angle,float angle, float integral_error[5]/*,int velocity */)
{
	float velocity=60; // shud be deleted
	int value1,value2; 						//Values to the wheels
	float error, difference, integral, derivative,time = 0.014;
	float kp, ki = 0.01;
	kp = 0.2;
	char command[4];
	command[0] = (char)(48+bot_id);
char *bot1_buffer = (char *)malloc(BUFFER_SIZE*sizeof(char));
#define BUFFER_SIZE 8
	difference = bot_angle;// - angle;
	if(abs(difference)<2)
	{
			command[1] = 's';
			command[2] = '0';
			command[3] = 's';
	}
	else
	{
//			difference = bot_angle;// - angle;
//			if((difference>90) || (difference<-90))
//			{
//				difference=-(180 + angle - bot_angle);
//			}	
			//cout<<"diff"<<difference<<endl;		
			//integral_error[bot_id] = integral_error[bot_id] + difference*time;
			//derivative = (difference - prev_error[bot_id])/time;
			if(abs(difference)<30){ 
			error = (kp*difference);// + (ki*integral_error[0]);//+ (kd*derivative);
			
			
			//integral_error[0]+=error;
			//cout<<"error"<<error<<endl;
			
				
			value1= abs(error);
			value2=value1;
			if(value1>velocity){
				value1 = velocity;
				value2=velocity;
			}
			}
			else{
				value1 = 20;
				value2 = 20;
			}
			//cout<<"initial"<<left<<":"<<right<<endl;
			if(difference>0)
			{
				command[1] = 'L';
				command[2] = (char)value1;
				command[3] = (char)value2;
			}
			if(difference<0)
			{
			command[1] = 'R';
				command[2] = (char)value1;
				command[3] = (char)value2;
			}	
			
		}
		//cout<<value1<<endl;
//		tcpWrite(socket,command);
//		send_command(command);
 // printf("Got : %s \n", tcpRead(socket,bot1_buffer));
//		cout<<command<<'\n';	
}

/*
move bot along arc

*/
//void move_arc(int bot_id, int socket, float curr_pos[2], float next_pos[2], int theta, float time){
//	float d = distance(curr_pos, next_pos);//TODO:Write this function
//	float R = d/(2*sin(theta));
//	int fwd = 0, bck = 0;
//	float b = 0.75; // Bot width
//	float r = 0.03; //Radius of wheel
//	float omega;
////	Arc Decision
//	if(abs(theta)<90){
//		fwd = 1;
//		bck = 0;
//	}
//	else{
//		fwd = 0;
//		bck = 1;	
//	}
//	
////	Calculating omega of bot
//	omega = 2*R*theta/time;
//	if(fwd == 1){
//		left = (R+(b/2))*omega/r;
//		right = (R-(b/2))*omega/r;
//	}
//	else{
//		left = (R-(b/2))*omega/r;
//		right = (R+(b/2))*omega/r;	
//	}
////	TODO:Send command to elec	

//}






























