#include "scene.h"
#include <string>
#include <GL/GLAUX.H>
#include <GL/glut.h>

using namespace std;

//textures
unsigned int texture[TX_NUM];

void Scene::init()
{
	AUX_RGBImageRec* textureImg = NULL;
	char* img_name[TX_NUM] = { "data/hills_bk.bmp","data/hills_rt.bmp",
		"data/hills_ft.bmp","data/hills_lf.bmp","data/hills_up.bmp",
		"data/hills_dn.bmp", "data/fireworkbox.bmp", "data/firework_red.bmp",
		"data/firework_blue.bmp", "data/firework_green.bmp", "data/firework_yellow.bmp",
		"data/firework_dark_red.bmp","data/firework_dark_blue.bmp", "data/firework_dark_green.bmp", 
		"data/firework_dark_yellow.bmp", "data/trunk.bmp"};

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

	memset(grid, 0, sizeof(grid));
}

void Scene::render()
{
	renderSkybox();
}

void Scene::renderSkybox()
{
	glPushMatrix();
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat[0]);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat[1]);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat[2]);
		glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
		
		//back surface
		glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(SCENE_SIZE, 0, 0);
		glVertex3f(SCENE_SIZE, SCENE_HT, 0);
		glVertex3f(0, SCENE_HT, 0);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glEnd();

		//right surface
		glBegin(GL_QUADS);
		glVertex3f(SCENE_SIZE, 0, 0);
		glVertex3f(SCENE_SIZE, 0, SCENE_SIZE);
		glVertex3f(SCENE_SIZE, SCENE_HT, SCENE_SIZE);
		glVertex3f(SCENE_SIZE, SCENE_HT, 0);
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glEnd();

		//front surface
		glBegin(GL_QUADS);
		glVertex3f(SCENE_SIZE, 0, SCENE_SIZE);
		glVertex3f(0, 0, SCENE_SIZE);
		glVertex3f(0, SCENE_HT, SCENE_SIZE);
		glVertex3f(SCENE_SIZE, SCENE_HT, SCENE_SIZE);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glEnd();

		//left surface
		glBegin(GL_QUADS);
		glVertex3f(0, 0, SCENE_SIZE);
		glVertex3f(0, 0, 0);
		glVertex3f(0, SCENE_HT, 0);
		glVertex3f(0, SCENE_HT, SCENE_SIZE);
		glNormal3f(1.0f, 0.0f, 0.0f);
		glEnd();

		//top surface
		glBegin(GL_QUADS);
		glVertex3f(0, SCENE_HT, 0);
		glVertex3f(SCENE_SIZE, SCENE_HT, 0);
		glVertex3f(SCENE_SIZE, SCENE_HT, SCENE_SIZE);
		glVertex3f(0, SCENE_HT, SCENE_SIZE);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glEnd();

		//bottom surface
		glBegin(GL_QUADS);
		glVertex3f(0, 0, SCENE_SIZE);
		glVertex3f(SCENE_SIZE, 0, SCENE_SIZE);
		glVertex3f(SCENE_SIZE, 0, 0);
		glVertex3f(0, 0, 0);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glEnd();
	}

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	{
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
	}

	glDisable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
	glPopMatrix();
}

bool Scene::occupy(int x, int z, int type) 
{
	switch (type)
	{
	case 1:
		if (grid[x][z] == 0) {
			grid[x][z] = 1;
			return true;
		}
		break;
	case 2:
		if (grid[x][z] == 0 && grid[x + 1][z] == 0
			&& grid[x][z + 1] == 0 && grid[x + 1][z + 1] == 0) 
		{
			grid[x][z] = grid[x + 1][z] = grid[x][z + 1] = grid[x + 1][z + 1] = 2;
			return true;
		}
		break;
	default: break;
	}
	return false;
}

bool Scene::release(int x, int z, int type)
{
	switch (type)
	{
	case 1:
		if (grid[x][z] == 1) {
			grid[x][z] = 0;
		}
		return true;
		break;
	case 2:
		if (grid[x][z] == 2 && grid[x + 1][z] == 2
			&& grid[x][z + 1] == 2 && grid[x + 1][z + 1] == 2) {
			grid[x][z] = grid[x + 1][z] = grid[x][z + 1] = grid[x + 1][z + 1] = 0;
		}
		else if (grid[x][z] == 2 && grid[x - 1][z] == 2
			&& grid[x - 1][z + 1] == 2 && grid[x][z + 1] == 2) {
			grid[x][z] = grid[x - 1][z] = grid[x - 1][z + 1] = grid[x][z + 1] = 0;
		}
		else if (grid[x][z] == 2 && grid[x][z - 1] == 2
			&& grid[x - 1][z - 1] == 2 && grid[x - 1][z] == 2) {
			grid[x][z] = grid[x][z - 1] = grid[x - 1][z - 1] = grid[x - 1][z] = 0;
		}
		else if (grid[x][z] == 2 && grid[x][z-1] == 2
			&& grid[x + 1][z - 1] == 2 && grid[x + 1][z] == 2) {
			grid[x][z] = grid[x][z - 1] = grid[x + 1][z - 1] = grid[x + 1][z] = 0;
		}
		return true;
		break;
	default: break;
	}
	return false;
}