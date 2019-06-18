#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_
#include <gl/glut.h>

GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
float m_x = 0;
float m_y = 0;
float m_z = 0;

extern float xpos, zpos, heading, piover180;
extern float z_roty,z_rotx,z_rotz,s_rotx,s_roty,s_rotz;

#endif