/*
**We shall use the direction of scoring as zero angle. And angle varies from 0 to 360
*/

#include"wifi.h"

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
	if(angle>45 && angle<135)
		command = {(char)(48+bot_id),'R','~','~'};	//rotate to the right
	else if(angle>225 && angle<315)
		command = {(char)(48+bot_id),'L','~','~'};	//rotate to the left
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
	command = {(char)(48+bot_id),F,'~','~'};
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
*/
void movement(int bot_id, int socket, float bot_angle,float angle, float dist)
