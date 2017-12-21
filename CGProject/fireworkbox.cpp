#include "fireworkbox.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

//flag to emit firework particles for endless mode
int flag_endless = 0;


Fireworkbox::Fireworkbox(float p_x, float p_z)
	:pos_x(p_x), pos_z(p_z)
{
	flag_emit = dead_firework = 0;
	fireworks = new Firework*[FP_PER_BOX];
	int fp_per_row = FBOX_SIZE / 2;
	for (int i = 0; i < fp_per_row; ++i) {
		for (int j = 0; j < fp_per_row; ++j) {
			int posx = pos_x - fp_per_row + 2 * i + 1;
			int posz = pos_z - fp_per_row + 2 * j + 1;
			fireworks[i * fp_per_row + j] = new Firework(posx, posz);
		}
	}
}

Fireworkbox::~Fireworkbox()
{
	if (fireworks != NULL) {
		delete []fireworks;
	}
}

void Fireworkbox::update()
{
	if (fireworks == NULL) {
		if (flag_endless) {
			fireworks = new Firework*[FP_PER_BOX];
			int fp_per_row = FBOX_SIZE / 2;
			for (int i = 0; i < fp_per_row; ++i) {
				for (int j = 0; j < fp_per_row; ++j) {
					int posx = pos_x - fp_per_row + 2 * i + 1;
					int posz = pos_z - fp_per_row + 2 * j + 1;
					fireworks[i * fp_per_row + j] = new Firework(posx, posz);
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
		} else if (fireworks[i]->getStat() == INIT && flag_emit > 16) {
			flag_emit = 0;
			fireworks[i]->activate();
		} else if (fireworks[i]->getStat() == DEAD) {
			++dead_firework;
			fireworks[i]->wait();
		}
	}
}

void Fireworkbox::render()
{
	glPushMatrix();
	glTranslatef(pos_x, 0, pos_z);
	glutSolidCube(GRID_SIZE);
	glPopMatrix();
	if (fireworks == NULL) return;
	for (int i = 0; i < FP_PER_BOX; ++i) {
		if (fireworks[i] != NULL && fireworks[i]->getStat() == ACTIVE) {
			fireworks[i]->render();
		}
	}
}

Fireworkbox fireworkbox(90,90);

void renderFireworkboxs()
{
	fireworkbox.render();
}

void updateFireworkboxs()
{
	fireworkbox.update();
}