// sanwei.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <GL/glut.h>
#include "math.h"
#include "movement.h"


GLfloat vertices[][3] = { { -2.0,-2.0,-2.0 },{ 2.0,-2.0,-2.0 },{ -2.0,2,-2.0 },
{ 2.0,2,-2.0 },{ -2.0,-2.0,2.0 },{ 2.0,-2.0,2.0 },
{ 2.0,2,2.0 },{ -2.0,2,2.0 } };
GLfloat vertices2[][3] = { { -3.05,-2,-2 },{ 3.05,-2,-2 },{ 0,-2,3.29 },{ 0,3.1,0.6 } };
GLfloat colors[][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 },{ 1.0,1.0,0.0 },
{ 0.0,1.0,1.0 },{ 1.0,0.0,1.0 },{ 0.0,0.0,0.0 },{ 0.0,1.0,1.0 } };
float thex = 0;
float they = 0;
extern float m_x;
extern float m_y;
extern float m_z;
float z_roty = 0;
float z_rotx = 0;
float z_rotz = 0;
float s_roty = 0;
float s_rotx = 0;
float s_rotz = 0;

int window_height = 500, window_width = 500;


const float piover180 = 0.0174532925f;
float heading;
float xpos;
float zpos = 15.0;

const GLfloat defaultambient[] = { 0.2,0.2,0.2,1.0 };
const GLfloat defaultspeculator[] = { 0,0,0,1.0 };
const GLint SpeculatorIndex = 50;
const GLfloat ambientAndDiffuse[] = { 0.2,0.5,0.8 };
const GLfloat ambientAndDiffuseVPot[] = { 1,0.84,0 };

extern GLfloat	yrot;				// Y Rotation
extern GLfloat walkbias;
extern GLfloat walkbiasangle;
extern GLfloat lookupdown;

static GLfloat theta[] = { 0.0,0.0,0.0 };
static GLint axis = 2;
static GLdouble viewer[] = { 0.0, 0.0, -5.0 };
void colorteapot2()
{

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ambientAndDiffuseVPot);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambientAndDiffuseVPot);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientAndDiffuseVPot);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	glutSolidTeapot(1);
}

void colorteapot()
{

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultspeculator);
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, SpeculatorIndex);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientAndDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ambientAndDiffuse);

	glutSolidTeapot(1);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	refreshView();
	refreshLights();

	drawFlashlight();

	glPushMatrix();
	glTranslatef(0, 0, -5);
	//glRotatef(z_rotx,1.0f,0,0);
	//glRotatef(z_roty,0.0f,1.0,0);
	//glRotatef(z_rotz,0.0f,0,1.0);
	colorteapot();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 2, 5);
	//glRotatef(s_rotx,1.0f,0,0);
	//glRotatef(s_roty,0.0f,1.0,0);
	//glRotatef(s_rotz,0.0f,0,1.0);
	colorteapot2();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0, -3, 2);
	colorteapot2();
	glPopMatrix();


	if (DEBUG & DB_flashLightMove) {
		glPushMatrix();
		glTranslatef(0, 6, 5);
		glutSolidCube(0.2);
		glPopMatrix();
	}

	//	glRotatef(lookupdown,1.0f,0,0);
	//	glRotatef(sceneroty,0,1.0f,0);


	// glTranslatef(5.0, -2, -5);
	// glBegin(GL_POLYGON);  glColor3fv(colors[1]); 
	// glVertex3f(-10,0,-20); 
	// glVertex3f(10,0,-20); 
	// glVertex3f(10,0,20); 
	// glVertex3f(-10,0,20); 
	// glEnd();
	glutSwapBuffers();
}
void mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
	theta[axis] += 2.0;
	if (theta[axis] > 360.0) theta[axis] -= 360.0;
	display();
}

void keys(unsigned char key, int x, int y)
{
	movement_keys(key, x, y);
}

void idle()
{
	glutPostRedisplay();
}
void myReshape(int w, int h)
{
	window_height = h;
	window_width = w;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	gluPerspective(45.0, w / h, 1.0, 50.0);
	//glOrtho(-10, 10, -10, 10, -100, 100);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("colorcube");
	initMovement();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	//glutMouseFunc(mouse); 
	glutPassiveMotionFunc(passiveMouse);
	glutKeyboardFunc(keys);
	glutIdleFunc(idle);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}
