#include "light.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/GLUT.H>


GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f }; 
GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specularLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat lightPos[] = { 50.0f, 100.0f, 50.0f, 0.0f };

Light::Light()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void Light::init()
{
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight); 
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight); 

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularLight);  
	glMateriali(GL_FRONT, GL_SHININESS, 100);
}

void Light::adjustPos()
{
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}

void Light::adjustAmbient()
{
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
}

void Light::adjustDiffuse()
{
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
}

void Light::adjustSpecular()
{
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
}