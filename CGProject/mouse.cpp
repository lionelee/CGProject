#include "mouse.h"
#include "camera.h"

bool right_clicked = false;
int mousex = 0, mousey = 0;

extern Camera camera;

void mouseFunc(int button, int state, int x, int y)
{
	switch (button){
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			right_clicked = true; 
			mousex = x;
			mousey = y;
		}
		else if(state == GLUT_UP)right_clicked = false;
	default: break;
	}
}

void motionFunc(int x, int y)
{
	if (right_clicked) {
		int dx = x - mousex, dy = y - mousey;
		if (dx < 0)camera.rotate(LEFT, -dx);
		if (dx > 0)camera.rotate(RIGHT, dx);
		if (dy < 0)camera.rotate(UP, -dy);
		if (dy > 0)camera.rotate(DOWN, dy);
		mousex = x;
		mousey = y;
	}
}

void initMouse() 
{
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
}