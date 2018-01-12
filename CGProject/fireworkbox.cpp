#include "fireworkbox.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <GL/glut.h>
#include "light.h"

using namespace std;

extern Light light;

//flag to emit firework particles for endless mode
int flag_endless = 0;
//flag to stop emission
int flag_stop = 0;

extern unsigned int texture[TX_NUM];

Fireworkbox::Fireworkbox(int p_x, int p_z, int _layer, int _type)
	:pos_x(p_x), pos_z(p_z), layer(_layer), type(_type)
{
	flag_emit = dead_firework = 0;
	fireworks = new Firework*[FP_PER_BOX];
	int fp_per_row = FBOX_SIZE / 2;
	amb = light.accquireLightSrc();
	for (int i = 0; i < fp_per_row; ++i) {
		for (int j = 0; j < fp_per_row; ++j) {
			int posx = pos_x - fp_per_row + 2 * i + 1;
			int posz = pos_z - fp_per_row + 2 * j + 1;
			int tx = rand() % 4 + 7;
			fireworks[i * fp_per_row + j] = new Firework(posx, posz, texture[tx], layer, type, amb);
		}
	}
}

Fireworkbox::~Fireworkbox()
{
	if (fireworks != NULL) {
		delete []fireworks;
	}
	light.releaseLightSrc(amb);
}

void Fireworkbox::update()
{
	if (flag_stop)return;
	if (fireworks == NULL) {
		if (flag_endless) {
			fireworks = new Firework*[FP_PER_BOX];
			int fp_per_row = FBOX_SIZE / 2;
			for (int i = 0; i < fp_per_row; ++i) {
				for (int j = 0; j < fp_per_row; ++j) {
					int posx = pos_x - fp_per_row + 2 * i + 1;
					int posz = pos_z - fp_per_row + 2 * j + 1;
					int tx = rand() % 4 + 7;
					fireworks[i * fp_per_row + j] = new Firework(posx, posz, texture[tx], layer, type, amb);
				}
			}
		} else {
			return;
		}
	}
	++flag_emit;
	if (dead_firework == FP_PER_BOX) {				//emission done for one round
		if (flag_endless) {
			dead_firework = 0;
			for (int i = 0; i < FP_PER_BOX; ++i)	 fireworks[i]->init();
		} else {
			for (int i = 0; i < FP_PER_BOX; ++i) {
				delete fireworks[i];
				fireworks[i] = NULL;
			}
			delete []fireworks;
			fireworks = NULL;
		}
		return;
	}
	for (int i = 0; i < FP_PER_BOX; ++i) {
		if (fireworks[i] == NULL) continue;
		if (fireworks[i]->getStat() == ACTIVE) {
			fireworks[i]->update();
		} else if (fireworks[i]->getStat() == INIT && flag_emit > FP_EMT_INT) {
			flag_emit = 0;
			fireworks[i]->activate();
		} else if (fireworks[i]->getStat() == DEAD) {
			++dead_firework;
			fireworks[i]->wait();
		}
	}
}

void Fireworkbox::renderBox()
{
	glPushMatrix();
	glTranslatef(pos_x, GRID_SIZE / 2, pos_z);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat[2]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);
	
	glutSolidCube(GRID_SIZE); 

	float k = GRID_SIZE / 2.0;
	glEnable(GL_TEXTURE_2D); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-k, -k, -k);
	glTexCoord2f(1, 0); glVertex3f(-k, -k, k);
	glTexCoord2f(1, 1); glVertex3f(-k, k, k);
	glTexCoord2f(0, 1); glVertex3f(-k, k, -k);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-k, -k, k);
	glTexCoord2f(1, 0); glVertex3f(k, -k, k);
	glTexCoord2f(1, 1); glVertex3f(k, k, k);
	glTexCoord2f(0, 1); glVertex3f(-k, k, k);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(k, -k, k);
	glTexCoord2f(1, 0); glVertex3f(k, -k, -k);
	glTexCoord2f(1, 1); glVertex3f(k, k, -k);
	glTexCoord2f(0, 1); glVertex3f(k, k, k);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(k, -k, -k);
	glTexCoord2f(1, 0); glVertex3f(-k, -k, -k);
	glTexCoord2f(1, 1); glVertex3f(-k, k, -k);
	glTexCoord2f(0, 1); glVertex3f(k, k, -k);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Fireworkbox::render()
{
	renderBox();
	if (flag_stop)return;
	if (fireworks == NULL) return;
	for (int i = 0; i < FP_PER_BOX; ++i) {
		if (fireworks[i] != NULL && fireworks[i]->getStat() == ACTIVE) {
			fireworks[i]->render();
		}
	}
}

list<Fireworkbox*> boxes;
void addFireworkbox(int p_x, int p_z, int _layer, int _type)
{
	if (boxes.size() > 7)return;
	boxes.push_back(new Fireworkbox(p_x, p_z, _layer, _type));
}

void removeFireworkbox(int p_x, int p_z)
{
	list<Fireworkbox*>::iterator it = boxes.begin();
	for (; it != boxes.end(); ++it) {
		if ((*it)->getPosx() == p_x && (*it)->getPosz() == p_z) {
			delete (*it);
			boxes.erase(it);
			return;
		}
	}
}

void renderFireworkboxes()
{
	list<Fireworkbox*>::iterator it = boxes.begin();
	for (; it != boxes.end(); ++it)
		(*it)->render();
}

void updateFireworkboxes()
{
	list<Fireworkbox*>::iterator it = boxes.begin();
	for (; it != boxes.end(); ++it)
		(*it)->update();
}