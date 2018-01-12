#include "keyboard.h"
#include "camera.h"
#include "light.h"

extern Camera camera;
extern Light light;

void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
	case 'a': case'A':
		camera.move(LEFT);
		break;
	case 'd': case 'D':
		camera.move(RIGHT);
		break;
	case 'w': case 'W':
		camera.move(FORWARD);
		break;
	case 's': case 'S':
		camera.move(BACKWARD);
		break;
	case 'q': case 'Q':
		camera.move(UP);
		break;
	case 'e': case 'E':
		camera.move(DOWN);
		break;
	case 'j': case 'J':
		light.move(LLEFT);
		break;
	case 'l': case 'L':
		light.move(LRIGHT);
		break;
	case 'i': case 'I':
		light.move(LFORWARD);
		break;
	case 'k': case 'K':
		light.move(LBACKWARD);
		break;
	case 'u':case'U':
		light.move(LUP);
		break;
	case 'o':case'O':
		light.move(LDOWN);
		break;
	default: break;
	}
}

void specialFunc(int key, int x, int y)
{
	switch (key){
	case GLUT_KEY_LEFT:
		camera.rotate(LEFT , 10);
		break;
	case GLUT_KEY_RIGHT:
		camera.rotate(RIGHT, 10);
		break;
	case GLUT_KEY_UP:
		camera.rotate(UP, 10);
		break;
	case GLUT_KEY_DOWN:
		camera.rotate(DOWN, 10);
		break;
	default: break;
	}
}

void initKeyboard()
{
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);
}