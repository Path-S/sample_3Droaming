#include "stdafx.h"
#include <math.h>
#include "movement.h"


GLfloat eye[3] = { 0,0,10 }, viewCenter[3]{ 0,0,9 },head_up[3] = {0,1,0};
bool qed = false, eed = false;
int lastX = 0,lastY = 0;
float viewAngleX = 0,viewAngleY = 0;
RECT mainWindowRect;

const float move_step = 0.5;

void initMovement() {
	POINT curPoint;
	GetCursorPos(&curPoint);
	HWND mainWindow = WindowFromPoint(curPoint);
	mainWindowRect.left = mainWindowRect.right = mainWindowRect.bottom = mainWindowRect.top = 0;
	GetWindowRect(mainWindow, &mainWindowRect);
	if (DEBUG & DB_initMovement) {
		printf("\n mainWindowRect.left = %d, top = %d\n(x,y) = (%d,%d)\n", mainWindowRect.left, mainWindowRect.top,curPoint.x,curPoint.y);
	}
}

int fixMouse(int &x, int &y){
	initMovement();
	bool flag = false;
	//the x,y of windows.h and openGL are different. I need to fix this difference
	POINT curPoint;
	GetCursorPos(&curPoint);
	int deltaX = curPoint.x - x;
	int deltaY = curPoint.y - y;
	if(x < window_width/4){
		flag = true;
		lastX += window_width/4;
		x += window_width/4;
		SetCursorPos(deltaX+x,deltaY +y);
	}
	else if(x > 3*window_width/4){
		flag = true;
		lastX -= window_width/4;
		x -= window_width/4;
		SetCursorPos(deltaX + x,deltaY + y);
	}
	if(y < window_height/4){
		flag = true;
		lastY += window_height/4;
		y += window_height/4;
		SetCursorPos(deltaX + x, deltaY + y);
	}
	else if(y >3*window_height/4){
		flag = true;
		lastY -= window_height/4;
		y -= window_height/4;
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
	if(lastX == 0 && lastY == 0){
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

	if(DEBUG)
	{
		printf("\nafter finxMouse(), (x,y) = (%d,%d),   (lastX,lastY) = (%d,%d)\n",x,y,lastX,lastY);
	}
	if(!isFixed){
		displacementX = x - lastX;
		displacementY = y - lastY;
		lastY = y;
		lastX = x;

		viewAngleX += ((float)displacementX / (float)window_width) * 2 * PI;
		viewAngleY -= ((float)displacementY / (float)window_height) * 2 * PI;
		
		

		if (viewAngleX > 2 * PI) viewAngleX -= 2 * PI;
		if (viewAngleY > 2 * PI) viewAngleY -= 2 * PI;

		viewCenter[0] = eye[0] + 1 * cos(viewAngleY) * sin(viewAngleX);
		viewCenter[1] = eye[1] + 1 * sin(viewAngleY);
		viewCenter[2] = eye[2] - 1 * cos(viewAngleY) * cos(viewAngleX);
	}
	if(DEBUG){
		printf("angleX = %f PI, angleY = %f PI\n", viewAngleX /PI,viewAngleY/PI);
	}
}


void keys(unsigned char key, int x, int y)
{/* 用 x, X, y, Y, z, and Z 键 移动视点 */   
	if(key == 's') 
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
	if(key == 'w') 
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
	if (key=='a')
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
	if (key=='q')
	{
		qed = !qed;
		head_up[0] = qed?0.5:0;
		if (DEBUG) {
			printf("head_up:%f,%f,%f\n", head_up[0],head_up[1],head_up[2]);
		}
	}
	if (key=='d')
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
	if (key=='e')
	{
		eed = !eed;
		head_up[0] = eed?-0.5:0;
		if (DEBUG) {
			printf("head_up:%f,%f,%f\n", head_up[0],head_up[1],head_up[2]);
		}
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
	//caller shoud call display() after change state;
}

void refreshView()
{
	gluLookAt(eye[0], eye[1], eye[2],
		viewCenter[0], viewCenter[1], viewCenter[2],
		head_up[0], head_up[1], head_up[2]);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
}