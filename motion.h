#include "wifi.h"

void shoot(float angle,int bot_id,int socket);
void pass(int bot_id,int socket);
bool stopped(int bot_id, int socket, int curr[2], float tol);
int movement(int bot_id, int socket, float bot_angle,float angle, float dist, float integral_error[5], int flag[2], float prev_error[5],int velocity,int stop,float tol);
void turn(int bot_id, int socket, float bot_angle,float angle, float integral_error[5]);
