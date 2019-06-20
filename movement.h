#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_
#include <gl/glut.h>
#include <stdio.h>
#include <windows.h>


extern float xpos, zpos, heading;
extern float z_roty,z_rotx,z_rotz,s_rotx,s_roty,s_rotz;
extern const float piover180;
extern int window_height, window_width;

void keys(unsigned char key, int x, int y);
void refreshView();
void passiveMouse(int x, int y);
void initMovement();

#define DB_initMovement 0
#define MOUSE_MOVE_GAP 50
#define DEBUG 1
#define PI 3.1415926
#endif