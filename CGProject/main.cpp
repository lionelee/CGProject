#define GLUT_DISABLE_ATEXIT_HACK

#include "macros.h"
#include <stdlib.h>
#include <time.h>
#include "gui.h"
#include "scene.h"
#include "particle.h"
#include "fireworkbox.h"
#include "light.h"
#include "camera.h"
#include "keyboard.h"

GUI gui;
Scene scene;
Light light;
Camera camera;
int main_window;
int time_now = 0, time_prev = 0;

void initialize(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glutInitWindowPosition(WIN_POSX, WIN_POSY);
	glutInitWindowSize(WIN_LENGTH, WIN_WIDTH);
	main_window = glutCreateWindow("CGProject");

	//glClearColor(0.5, 0.9, 0.9, 1.0);
}

void reshapeFunc(int w, int h) 
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (h == 0) h = 1;
	gluPerspective(90, (GLfloat)w / (GLfloat)h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera.look();
}

void displayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
	//renderParticles();
	//updateParticles();
	renderFireworkboxs();
	updateFireworkboxs();
	glutSwapBuffers();

	time_now = glutGet(GLUT_ELAPSED_TIME);
	while (time_now - time_prev < 16) {
		time_now = glutGet(GLUT_ELAPSED_TIME);
	}
	time_prev = time_now;

	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	initialize(argc, argv);
	gui.init(main_window);
	light.init();
	camera.init();
	initKeyboard();
	scene.init();
	srand(time(NULL));

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	//glutIdleFunc(displayFunc);
	glutMainLoop();
	return 0;
}