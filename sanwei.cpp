// sanwei.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "math.h"
GLfloat vertices[][3] = {{-2.0,-2.0,-2.0},{2.0,-2.0,-2.0},{-2.0,2,-2.0},
{2.0,2,-2.0}, {-2.0,-2.0,2.0},{2.0,-2.0,2.0}, 
{2.0,2,2.0}, {-2.0,2,2.0}};
GLfloat vertices2[][3]={{-3.05,-2,-2},{3.05,-2,-2},{0,-2,3.29},{0,3.1,0.6}};
GLfloat colors[][3] = {{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}, {1.0,1.0,0.0}, 
{0.0,1.0,1.0},{1.0,0.0,1.0}, {0.0,0.0,0.0}, {0.0,1.0,1.0}};
float thex=0;
float they=0;
float m_x=0;
float m_y=0;
float m_z=0;
float z_roty=0;
float z_rotx=0;
float z_rotz=0;
float s_roty=0;
float s_rotx=0;
float s_rotz=0;


const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos=15.0;

GLfloat	yrot;				// Y Rotation
GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;
GLfloat lookupdown = 0.0f;
void three(int a,int b,int c,int d)
{
    glBegin(GL_POLYGON);  glColor3fv(colors[a]); 
	glVertex3fv(vertices2[b]); 
	glVertex3fv(vertices2[c]);  
	glVertex3fv(vertices2[d]); 

	glEnd();

}
void polygon(int a, int b, int c , int d,int f)
{ 
    glBegin(GL_POLYGON);  glColor3fv(colors[a]); 
	glVertex3fv(vertices[b]); 
	glVertex3fv(vertices[c]);  glVertex3fv(vertices[d]); 
	glVertex3fv(vertices[f]);
	glEnd();
}
static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2;
static GLdouble viewer[]= {0.0, 0.0, -5.0}; 
void colorcube()
{
	polygon(0,0,1,3,2);   polygon(7,4,5,6,7);   polygon(2,1,3,6,5);
	polygon(3,0,2,7,4);   polygon(4,0,1,5,4);   polygon(5,2,3,6,7);
}
void smti()
{
	three(0,0,1,2);
	three(2,0,1,3);
	three(4,1,2,3);
	three(3,0,2,3);
}
void display(){ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity();
	GLfloat x_m, y_m, z_m, u_m, v_m;
	GLfloat xtrans = -xpos;
	GLfloat ztrans = -zpos;
	GLfloat ytrans = -walkbias-2.25f;
	GLfloat sceneroty = 360.0f - yrot;

	
	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);

	
	glTranslatef(-5.0+xtrans,0.1+ytrans+2,-10+ztrans+15);
	glRotatef(z_rotx,1.0f,0,0);
	glRotatef(z_roty,0.0f,1.0,0);
	glRotatef(z_rotz,0.0f,0,1.0);
	colorcube(); 
	glLoadIdentity();

	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
	glTranslatef(5.0+xtrans,0.1+ytrans+2,-10+ztrans+15);
	glRotatef(s_rotx,1.0f,0,0);
	glRotatef(s_roty,0.0f,1.0,0);
	glRotatef(s_rotz,0.0f,0,1.0);
	smti();
	glLoadIdentity();

	glRotatef(lookupdown,1.0f,0,0);
	glRotatef(sceneroty,0,1.0f,0);
	glTranslatef(xtrans, ytrans, ztrans);
    glBegin(GL_POLYGON);  glColor3fv(colors[1]); 
	glVertex3f(-10,0,-20); 
	glVertex3f(10,0,-20); 
	glVertex3f(10,0,20); 
	glVertex3f(-10,0,20); 
	glEnd();
	glLoadIdentity();

	glutSwapBuffers();
}
void mouse(int btn, int state, int x, int y)
{ 
    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0; 
    if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
    if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
    theta[axis] += 2.0; 
    if( theta[axis] > 360.0 ) theta[axis] -= 360.0; 
    display();
}
void keys(unsigned char key, int x, int y)
{/* 用 x, X, y, Y, z, and Z 键 移动视点 */   
	if(key == 's') 
	{
		xpos += (float)sin(heading*piover180) * 0.05f;
		zpos += (float)cos(heading*piover180) * 0.25f;
		if (walkbiasangle <= 1.0f)
		{
			walkbiasangle = 359.0f;
		}
		else
		{
			walkbiasangle-= 10;
		}
		walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
	}
	if(key == 'w') 
	{
		xpos -= (float)sin(heading*piover180) * 0.05f;
		zpos -= (float)cos(heading*piover180) * 0.25f;
		if (walkbiasangle >= 359.0f)
		{
			walkbiasangle = 0.0f;
		}
		else
		{
			walkbiasangle+= 10;
		}
		walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
	}
	if (key=='a')
	{
		heading += 3.0f;	
		yrot = heading;
	}
	if (key=='q')
	{
		lookupdown-= 1.0f;
	}
	if (key=='d')
	{
		heading -= 3.0f;	
		yrot = heading;
	}
	if (key=='e')
	{
		lookupdown+= 1.0f;
	}
	if (key=='z')
	{
		z_rotx+=10;
	}
	if (key=='x')
	{
		z_rotx-=10;
	}
	if (key=='c')
	{
		z_roty+=10;
	}
	if (key=='v')
	{
		z_roty-=10;
	}
	if (key=='b')
	{
		z_rotz+=10;
	}
	if (key=='n')
	{
		z_rotz-=10;
	}
	if (key=='1')
	{
		s_rotx+=10;
	}
	if (key=='2')
	{
		s_rotx-=10;
	}
	if (key=='4')
	{
		s_roty+=10;
	}
	if (key=='5')
	{
		s_roty-=10;
	}
	if (key=='7')
	{
		s_rotz+=10;
	}
	if (key=='8')
	{
		s_rotz-=10;
	}
	display();
}
void myReshape(int w, int h)
{ 
	glViewport(0, 0, w, h);
glMatrixMode(GL_PROJECTION); glLoadIdentity(); 
if(w<=h) glFrustum(-2.0, 2.0, -2.0 * (GLfloat) h/ (GLfloat) w,       
				   2.0* (GLfloat) h / (GLfloat) w, 2.0, 20.0); 
else glFrustum(-2.0, 2.0, -2.0 * (GLfloat) w/ (GLfloat) h,       
			   2.0* (GLfloat) w / (GLfloat) h, 2.0, 20.0);
/* 或者用gluPerspective(45.0, w/h, 1.0, 10.0); */ 
glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char **argv)
{ glutInit(&argc, argv); 
glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
glutInitWindowSize(500, 500); glutCreateWindow("colorcube");
glutReshapeFunc(myReshape); glutDisplayFunc(display); 
glutMouseFunc(mouse); glutKeyboardFunc(keys);
glEnable(GL_DEPTH_TEST); glutMainLoop();
}
