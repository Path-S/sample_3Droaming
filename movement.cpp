#include "stdafx.h"
#include <math.h>
#include "movement.h"


GLfloat eye[3] = { 0,0,10 }, viewCenter[3]{ 0,0,9 }, head_up[3] = { 0,1,0 };
bool qed = false, eed = false;
bool bFlashLight = false, bGeneralLight = false;

int lastX = 0, lastY = 0;
float viewAngleX = 0, viewAngleY = 0;
RECT mainWindowRect;

const float move_step = 0.5;
const float flashlight_deltaY = 0.5;
const float flashlight_deltaZ = 3;
const GLfloat ambientLightColor[] = { 0.2,0.2,0.2,1.0 };
const GLfloat flashLightAmbientColor[] = { 0,0,0,1.0 };
const GLfloat light_color[] = { 0.5,0.5,0.5,1 };
const GLfloat flash_light_color[] = { 1,1,1,1 };

GLfloat flashlight_pos[4];

GLfloat light_pos[4] = { 0,0,0,1 };
GLfloat LIGHT1Pos[] = { 0,5,0,1.0 };
GLfloat LIGHT1Dir[] = { 0,-1,0 };
GLfloat spotangle = 5;


void initMovement() {
	POINT curPoint;
	GetCursorPos(&curPoint);
	HWND mainWindow = WindowFromPoint(curPoint);
	mainWindowRect.left = mainWindowRect.right = mainWindowRect.bottom = mainWindowRect.top = 0;
	GetWindowRect(mainWindow, &mainWindowRect);
	SetCursorPos(window_width / 2, window_height / 2);

	if (!DEBUG) {
		glutSetCursor(GLUT_CURSOR_NONE);
	}

	if (DEBUG & DB_initMovement) {
		printf("\n mainWindowRect.left = %d, top = %d\n(x,y) = (%d,%d)\n", mainWindowRect.left, mainWindowRect.top, curPoint.x, curPoint.y);
	}
}

int fixMouse(int &x, int &y) {
	//initMovement();
	bool flag = false;
	//the x,y of windows.h and openGL are different. I need to fix this difference
	POINT curPoint;
	GetCursorPos(&curPoint);
	int deltaX = curPoint.x - x;
	int deltaY = curPoint.y - y;
	if (x < window_width / 4) {
		flag = true;
		lastX += window_width / 4;
		x += window_width / 4;
		SetCursorPos(deltaX + x, deltaY + y);
	}
	else if (x > 3 * window_width / 4) {
		flag = true;
		lastX -= window_width / 4;
		x -= window_width / 4;
		SetCursorPos(deltaX + x, deltaY + y);
	}
	if (y < window_height / 4) {
		flag = true;
		lastY += window_height / 4;
		y += window_height / 4;
		SetCursorPos(deltaX + x, deltaY + y);
	}
	else if (y >3 * window_height / 4) {
		flag = true;
		lastY -= window_height / 4;
		y -= window_height / 4;
		SetCursorPos(deltaX + x, deltaY + y);
	}

	if (DEBUG)
	{
		printf("\nAt fixMouse(), x = %d, y = %d", x, y);
	}

	return flag;
}

void passiveMouse(int x, int y)
{

	int displacementX, displacementY;
	if (lastX == 0 && lastY == 0) {
		lastX = x; lastY = y;			//untouched mouse
		return;
	}
	// if(x - lastX >= MOUSE_MOVE_GAP || lastX - x >= MOUSE_MOVE_GAP){
	// 	lastX = x;
	// 	return;
	// }
	// if(y - lastY >= MOUSE_MOVE_GAP || lastY - y >= MOUSE_MOVE_GAP){
	// 	lastY = y;
	// 	return;
	// }

	int isFixed = fixMouse(x, y);

	if (DEBUG)
	{
		printf("\nafter finxMouse(), (x,y) = (%d,%d),   (lastX,lastY) = (%d,%d)\n", x, y, lastX, lastY);
	}
	if (!isFixed) {
		displacementX = x - lastX;
		displacementY = y - lastY;
		lastY = y;
		lastX = x;

		viewAngleX += ((float)displacementX / (float)window_width) * 2 * PI;
		viewAngleY -= ((float)displacementY / (float)window_height) * 2 * PI;
		//like most FPS games, the angle on Y axis can't exceed [-0.5PI, 0.5PI]
		if(viewAngleY >= 0.499 * PI)
			viewAngleY = 0.499*PI;
		if(viewAngleY <= -0.499 * PI)
			viewAngleY = -0.499*PI;


		if (viewAngleX > 2 * PI) viewAngleX -= 2 * PI;
		if (viewAngleX < -2 * PI) viewAngleX += 2 * PI;
		if (viewAngleY > 2 * PI) viewAngleY -= 2 * PI;
		if (viewAngleY < -2 * PI) viewAngleY += 2 * PI;

		viewCenter[0] = eye[0] + 1 * cos(viewAngleY) * sin(viewAngleX);
		viewCenter[1] = eye[1] + 1 * sin(viewAngleY);
		viewCenter[2] = eye[2] - 1 * cos(viewAngleY) * cos(viewAngleX);
	}
	if (DEBUG) {
		printf("angleX = %f PI, %f d, angleY = %f PI, %f d\n", viewAngleX / PI,viewAngleX*180/PI, viewAngleY / PI, viewAngleY*180/PI);
	}
}


void movement_keys(unsigned char key, int x, int y)
{/* 用 x, X, y, Y, z, and Z 键 移动视点 */
	if (key == 's')
	{
		eye[2] += move_step * cos(viewAngleX);
		eye[0] -= move_step * sin(viewAngleX);
		viewCenter[2] += move_step * cos(viewAngleX);
		viewCenter[0] -= move_step * sin(viewAngleX);
		if (DEBUG) {
			printf("angleX = %f, angleY = %f \n", viewAngleX, viewAngleY);
			printf("eye: %f,%f,%f, viewCenter: %f,%f,%f\n", eye[0], eye[1], eye[2], viewCenter[0], viewCenter[1], viewCenter[2]);
		}
	}
	if (key == 'w')
	{
		eye[2] -= move_step * cos(viewAngleX);
		eye[0] += move_step * sin(viewAngleX);
		viewCenter[2] -= move_step * cos(viewAngleX);
		viewCenter[0] += move_step * sin(viewAngleX);
		if (DEBUG) {
			printf("angleX = %f, angleY = %f \n", viewAngleX, viewAngleY);
			printf("eye: %f,%f,%f, viewCenter: %f,%f,%f\n", eye[0], eye[1], eye[2], viewCenter[0], viewCenter[1], viewCenter[2]);
		}
	}
	if (key == 'a')
	{
		eye[0] -= move_step * cos(viewAngleX);
		eye[2] -= move_step * sin(viewAngleX);
		viewCenter[0] -= move_step * cos(viewAngleX);
		viewCenter[2] -= move_step * sin(viewAngleX);
		if (DEBUG) {
			printf("angleX = %f, angleY = %f \n", viewAngleX, viewAngleY);
			printf("eye: %f,%f,%f, viewCenter: %f,%f,%f\n", eye[0], eye[1], eye[2], viewCenter[0], viewCenter[1], viewCenter[2]);
		}
	}
	// if (key=='q')
	// {
	// 	qed = !qed;
	// 	head_up[0] = qed?0.5:0;
	// 	if (DEBUG) {
	// 		printf("head_up:%f,%f,%f\n", head_up[0],head_up[1],head_up[2]);
	// 	}
	// }
	if (key == 'd')
	{
		eye[0] += move_step * cos(viewAngleX);
		eye[2] += move_step * sin(viewAngleX);
		viewCenter[0] += move_step * cos(viewAngleX);
		viewCenter[2] += move_step * sin(viewAngleX);
		if (DEBUG) {
			printf("angleX = %f, angleY = %f \n", viewAngleX, viewAngleY);
			printf("eye: %f,%f,%f, viewCenter: %f,%f,%f\n", eye[0], eye[1], eye[2], viewCenter[0], viewCenter[1], viewCenter[2]);
		}
	}
	// if (key=='e')
	// {
	// 	eed = !eed;
	// 	head_up[0] = eed?-0.5:0;
	// 	if (DEBUG) {
	// 		printf("head_up:%f,%f,%f\n", head_up[0],head_up[1],head_up[2]);
	// 	}
	// }
	if (key == ' ')
	{
		eye[1] += move_step;
		viewCenter[1] += move_step;
		if (DEBUG) {
			printf("angleX = %f, angleY = %f \n", viewAngleX, viewAngleY);
			printf("eye: %f,%f,%f, viewCenter: %f,%f,%f\n", eye[0], eye[1], eye[2], viewCenter[0], viewCenter[1], viewCenter[2]);
		}
	}
	if (key == 'c')
	{
		eye[1] -= move_step;
		viewCenter[1] -= move_step;
		if (DEBUG) {
			printf("angleX = %f, angleY = %f \n", viewAngleX, viewAngleY);
			printf("eye: %f,%f,%f, viewCenter: %f,%f,%f\n", eye[0], eye[1], eye[2], viewCenter[0], viewCenter[1], viewCenter[2]);
		}
	}
	if (key == '=') {
		spotangle += 1;
		if (DEBUG)
		{
			printf("spotangle : %f\n", spotangle);
		}
		if (DEBUG && DB_flashLightMove) {
			printf("LIGHT1Pos = (%f, %f, %f)     LIGHT1Dir = (%f, %f, %f)\n", LIGHT1Pos[0], LIGHT1Pos[1], LIGHT1Pos[2], LIGHT1Dir[0], LIGHT1Dir[1], LIGHT1Dir[2]);
		}
	}
	if (key == '-') {
		spotangle -= 1;
		if (DEBUG)
		{
			printf("spotangle : %f\n", spotangle);
		}
		if (DEBUG && DB_flashLightMove) {
			printf("LIGHT1Pos = (%f, %f, %f)     LIGHT1Dir = (%f, %f, %f)\n", LIGHT1Pos[0], LIGHT1Pos[1], LIGHT1Pos[2], LIGHT1Dir[0], LIGHT1Dir[1], LIGHT1Dir[2]);
		}
	}
	if (key == '1') {
		if (DEBUG) {
			printf("1:General Light now %d, Flash light now %d\n", bGeneralLight, bFlashLight);
		}
		switchLight(GENERAL_LIGHT, !bGeneralLight);
		if (DEBUG) {
			printf("2:General Light now %d, Flash light now %d\n", bGeneralLight, bFlashLight);
		}
	}
	if (key == '2') {
		if (DEBUG) {
			printf("1:General Light now %d, Flash light now %d\n", bGeneralLight, bFlashLight);
		}
		switchLight(FLASH_LIGHT, !bFlashLight);
		if (DEBUG) {
			printf("2:General Light now %d, Flash light now %d\n", bGeneralLight, bFlashLight);
		}
	}
	if (key == 'q')
		exit(0);

	//caller shoud call display() after change state;
}

void refreshView()
{
	gluLookAt(eye[0], eye[1], eye[2],
		viewCenter[0], viewCenter[1], viewCenter[2],
		head_up[0], head_up[1], head_up[2]);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
}

void initLights()
{
	glEnable(GL_LIGHTING);

	switchLight(GENERAL_LIGHT, ON);
	switchLight(FLASH_LIGHT, ON);

	glLightfv(GL_LIGHT1, GL_AMBIENT, flashLightAmbientColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, flash_light_color);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, flash_light_color);

	//LIGHT1Pos[0] = eye[0];LIGHT1Pos[1] = eye[1];LIGHT1Pos[2] = eye[2];
	glLightfv(GL_LIGHT1, GL_POSITION, LIGHT1Pos);

	LIGHT1Dir[0] = 0; LIGHT1Dir[1] = -1; LIGHT1Dir[2] = 0;
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotangle);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LIGHT1Dir);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);

}

void refreshLights()
{
	glEnable(GL_LIGHTING);

	if (bFlashLight) {
		glEnable(GL_LIGHT1);
	}
	else {
		glDisable(GL_LIGHT1);
	}
	if (bGeneralLight) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}
	flashlight_pos[0] = eye[0] + (flashlight_deltaZ * cos(viewAngleY) + flashlight_deltaY * sin(viewAngleY)) * sin(viewAngleX);
	flashlight_pos[1] = eye[1] + flashlight_deltaZ * sin(viewAngleY) - flashlight_deltaY * cos(viewAngleY); 
	flashlight_pos[2] = eye[2] - (flashlight_deltaZ * cos(viewAngleY) + flashlight_deltaY * sin(viewAngleY)) * cos(viewAngleX);;

	LIGHT1Dir[0] = viewCenter[0] - eye[0]; LIGHT1Dir[1] = viewCenter[1] - eye[1]; LIGHT1Dir[2] = viewCenter[2] - eye[2];

	LIGHT1Pos[0] = flashlight_pos[0] + LIGHT1Dir[0]; LIGHT1Pos[1] = flashlight_pos[1] + LIGHT1Dir[1];
	LIGHT1Pos[2] = flashlight_pos[2] + LIGHT1Dir[2];


	glLightfv(GL_LIGHT1, GL_AMBIENT, flashLightAmbientColor);
	glLightfv(GL_LIGHT1, GL_SPECULAR, flash_light_color);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, flash_light_color);
	glLightfv(GL_LIGHT1, GL_POSITION, LIGHT1Pos);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotangle);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, LIGHT1Dir);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);


	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	
}

void switchLight(int light, int b)
{
	switch (light) {
	case 0: {
		if (b) {
			glEnable(GL_LIGHT0);
			bGeneralLight = TRUE;
			if (DEBUG && DB_lightSwitch) {
				printf("I change bGeneralLight to TRUE at switchLight\n");
			}
		}
		else
		{
			glDisable(GL_LIGHT0);
			bGeneralLight = FALSE;
			if (DEBUG && DB_lightSwitch) {
				printf("I change bGeneralLight to FALSE at switchLight\n");
			}
		}
		break;
	}
	case 1: {
		if (b) {
			glEnable(GL_LIGHT1);
			bFlashLight = TRUE;
			if (DEBUG && DB_lightSwitch) {
				printf("I change bFlashLight to TURE at switchLight\n");
			}

		}
		else
		{
			glDisable(GL_LIGHT1);
			bFlashLight = FALSE;
			if (DEBUG && DB_lightSwitch) {
				printf("I change bFlashLight to FALSE at switchLight\n");
			}

		}
		break;
	}

	}
}

void drawFlashlight()
{
	if (DEBUG & DB_solidFlashLightPos) {
		printf("flashlight solid pos: (%f, %f, %f)\n", flashlight_pos[0], flashlight_pos[1], flashlight_pos[2]);
	}

	// glPushMatrix();
	// glTranslatef(flashlight_pos[0], flashlight_pos[1], flashlight_pos[2]);
	
	// //glRotatef(viewAngleX * 180, 0, 1, 0);
	// //glRotatef(viewAngleY * 180, 1, 0, 0);
	
	// glScalef(1, 1, 2);
	// glutSolidCube(0.1);
	// glPopMatrix();

	glPushMatrix();
	glTranslatef(eye[0],eye[1],eye[2]);
	glRotatef(viewAngleX * 180/PI, 0, -1, 0);
	glRotatef(viewAngleY * 180/PI, 1, 0, 0);
	glTranslatef(0,-flashlight_deltaY,-flashlight_deltaZ);

	

	glScalef(1, 1, 2);
	glutSolidCube(0.1);
	glPopMatrix();

}