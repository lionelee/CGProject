#define GLUT_DISABLE_ATEXIT_HACK

#include "macros.h"
#include <stdlib.h>
#include <time.h>
#include "gui.h"
#include "scene.h"
#include "tree.h"
#include "particle.h"
#include "objmodel.h"
#include "fireworkbox.h"
#include "light.h"
#include "camera.h"
#include "mouse.h"
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
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutInitWindowPosition(WIN_POSX, WIN_POSY);
	glutInitWindowSize(WIN_LENGTH, WIN_WIDTH);
	main_window = glutCreateWindow("CGProject");
	glClearColor(0.0, 0.0, 0.0, 1.0);
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

	light.illuminate();
	glPushMatrix();
	/*renderParticles();
	updateParticles();*/
	renderTrees();
	updateTrees();
	renderFireworkboxes();
	updateFireworkboxes();
	scene.render();
	glPopMatrix();
	light.update();
	glutSwapBuffers();

	time_now = glutGet(GLUT_ELAPSED_TIME);
	while (time_now - time_prev < 32) {
		time_now = glutGet(GLUT_ELAPSED_TIME);
	}
	time_prev = time_now;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	initialize(argc, argv);
	gui.init(main_window);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);

	camera.init();
	scene.init();
	initObjModel(1);
	initMouse();
	initKeyboard();
	srand(time(NULL));

	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutMainLoop();
	return 0;
}