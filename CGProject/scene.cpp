#include "scene.h"
#include <string>
#include <GL/GLAUX.H>
#include <GL/glut.h>

using namespace std;

void Scene::init()
{
	AUX_RGBImageRec* textureImg = NULL;
	char* img_name[TX_NUM] = { "data/hills_bk.bmp","data/hills_rt.bmp",
		"data/hills_ft.bmp","data/hills_lf.bmp","data/hills_up.bmp",
		"data/hills_dn.bmp", "data/grass.bmp", "data/wall.bmp" };

	for (int i = 0; i < TX_NUM; i++) {
		textureImg = auxDIBImageLoadA(img_name[i]);
		if (textureImg) {
			glGenTextures(1, &texture[i]);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureImg->sizeX, 
				textureImg->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImg->data);
			free(textureImg->data);
			free(textureImg);
		}
	}
}

void Scene::render()
{
	renderSkybox();
}

void Scene::renderSkybox()
{
	glEnable(GL_TEXTURE_2D);
	//back surface
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0); glVertex3f(SCENE_SIZE, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(SCENE_SIZE, SCENE_HT, 0);
	glTexCoord2f(0, 1); glVertex3f(0, SCENE_HT, 0);
	glEnd();

	//right surface
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(SCENE_SIZE, 0, 0);
	glTexCoord2f(1, 0); glVertex3f(SCENE_SIZE, 0, SCENE_SIZE);
	glTexCoord2f(1, 1); glVertex3f(SCENE_SIZE, SCENE_HT, SCENE_SIZE);
	glTexCoord2f(0, 1); glVertex3f(SCENE_SIZE, SCENE_HT, 0);
	glEnd();

	//front surface
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(SCENE_SIZE, 0, SCENE_SIZE);
	glTexCoord2f(1, 0); glVertex3f(0, 0, SCENE_SIZE);
	glTexCoord2f(1, 1); glVertex3f(0, SCENE_HT, SCENE_SIZE);
	glTexCoord2f(0, 1); glVertex3f(SCENE_SIZE, SCENE_HT, SCENE_SIZE);
	glEnd();

	//left surface
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, SCENE_SIZE);
	glTexCoord2f(1, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(0, SCENE_HT, 0);
	glTexCoord2f(0, 1); glVertex3f(0, SCENE_HT, SCENE_SIZE);
	glEnd();

	//top surface
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(0, SCENE_HT, 0);
	glTexCoord2f(0, 0); glVertex3f(SCENE_SIZE, SCENE_HT, 0);
	glTexCoord2f(1, 0); glVertex3f(SCENE_SIZE, SCENE_HT, SCENE_SIZE);
	glTexCoord2f(1, 1); glVertex3f(0, SCENE_HT, SCENE_SIZE);
	glEnd();

	//bottom surface
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, SCENE_SIZE);
	glTexCoord2f(1, 0); glVertex3f(SCENE_SIZE, 0, SCENE_SIZE);
	glTexCoord2f(1, 1); glVertex3f(SCENE_SIZE, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, 0, 0);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}