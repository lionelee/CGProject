#include "keyboard.h"
#include "camera.h"

extern Camera camera;

void keyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
	case 'a':
		camera.move(LEFT);
		break;
	case 'd':
		camera.move(RIGHT);
		break;
	case 'w':
		camera.move(FORWARD);
		break;
	case 's':
		camera.move(BACKWARD);
		break;
	case 'q':
		camera.move(UP);
		break;
	case 'e':
		camera.move(DOWN);
		break;
	default: break;
	}
}

void specialFunc(int key, int x, int y)
{
	switch (key){
	case GLUT_KEY_LEFT:
		camera.look(LEFT);
		break;
	case GLUT_KEY_RIGHT:
		camera.look(RIGHT);
		break;
	case GLUT_KEY_UP:
		camera.look(UP);
		break;
	case GLUT_KEY_DOWN:
		camera.look(DOWN);
		break;
	default: break;
	}
}

void initKeyboard()
{
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);
}