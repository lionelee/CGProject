#include "firework.h"
#include "objmodel.h"
#include "macros.h"
#include <vector>
#include <iostream>
#include <GL/glut.h>

using namespace std;

//vector of firework boxs
vector<Firework*> fbox;
//flag of changing firework particle to obj model
int flag_ch2obj = 0;

extern int light_onff;

Firework::Firework(int posx, int posz, int tx, int _layer, int _type, unsigned int light)
	:texture(tx),layer(_layer), type(_type), bkg_amb(light)
{
	p_x = posx;
	p_z = posz;
	p_y = FBOX_SIZE;
	a_y = -FP_AIR_RES * GRAVITY;
	stat = INIT;
	if (type == 0) subtotal = 25;
	else subtotal = 4;
	switch (tx) {
	case 8:
		bkg[0] = 0.8f; bkg[1] = 0.0f; bkg[2] = 0.0f;
		break;
	case 9:
		bkg[0] = 0.0f; bkg[1] = 0.0f; bkg[2] = 0.8f;
		break;
	case 10:
		bkg[0] = 0.0f; bkg[1] = 0.8f; bkg[2] = 0.0f;
		break;
	case 11:
		bkg[0] = 0.8f; bkg[1] = 0.8f; bkg[2] = 0.0f;
		break;
	default: break;
	}
	bkg[3] = 1.0f;
}

Firework::~Firework()
{
	if (particles != NULL) {
		for (int i = 0; i < layer; ++i) {
			for (int j = 0; j < subtotal; ++j) {
				if (particles[i][j] != NULL)
					delete particles[i][j];
			}
			if (particles[i] != NULL)
				delete []particles[i];
		}
		delete []particles;
	}
}

void Firework::init()
{
	stat = INIT;
	p_y = FBOX_SIZE;
}

void Firework::activate()
{
	stat = ACTIVE;
	v_x = v_z = 0.0f; v_y = FP_V;
	radius = 0.5;
	dead_particle  = bkg_times = 0;
	flag_explosion = false;
	num = layer * subtotal;
	if (particles == NULL) {
		particles = new Particle**[layer];
		for (int i = 0; i < layer; ++i) {
			particles[i] = new Particle*[subtotal];
			for (int j = 0; j < subtotal; ++j) {
				if (type == 0) particles[i][j] = new Particle();
				else particles[i][j] = new ObjModel(type);
			}
		}
	}
}

void Firework::update()
{
	if (stat == DEAD || dead_particle == layer * subtotal) {
		stat = DEAD;
		return;
	}
	if (v_y <= 0) {									//explosion
		if (!flag_explosion) {
			if (type == 0) {
				activeParticles();
			} else {
				activeObjModels();
			}
			flag_explosion = true;
		}
		for (int i = 0; i < layer; ++i) {
			for (int j = 0; j < subtotal; ++j) {
				if (particles[i][j] == NULL) continue;
				if (particles[i][j]->getStat() == ACTIVE) {
					if(type == 0 && particles[i][j]->getUpdateTimes() > PA_DEA_INT) {
						particles[i][j]->wait();
						++dead_particle;
					} else {
						particles[i][j]->update();
					}
				} else if (particles[i][j]->getStat() == DEAD) {
					particles[i][j]->wait();
					++dead_particle;
				}
			}
		}
	} else {
		v_y += a_y;
		p_y += v_y;
	}
}

void Firework::wait()
{
	stat = WAIT;
	for (int i = 0; i < layer; ++i) {
		for (int j = 0; j < subtotal; ++j)
			particles[i][j]->init();
	}
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Firework::render()
{
	if (v_y <= 0) {									//explosion
		if (flag_explosion && type == 0) {
				//glClearColor(r, g, b, 1.0f);
			if (bkg_times < 16) {
				float pos[4] = { p_x, p_y, p_z, 1.0f };
				glLightfv(bkg_amb, GL_POSITION, pos);
				glLightf(bkg_amb, GL_CONSTANT_ATTENUATION, 5.0f);
				float diff[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
				glLightfv(bkg_amb, GL_DIFFUSE, diff);
				float spec[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
				glLightfv(bkg_amb, GL_SPECULAR, spec);
				glLightfv(bkg_amb, GL_AMBIENT, bkg);
				glEnable(bkg_amb);
				++bkg_times;
			} else {
				glDisable(bkg_amb);
			}
		}
		for (int i = 0; i < layer; ++i) {
			for (int j = 0; j < subtotal; ++j) {
				if (particles[i][j] != NULL && particles[i][j]->getStat() == ACTIVE) {
					if (type == 0) {
						if (flag_ch2obj > 0) {
							particles[i][j]->renderObj();
						} else {
							if (light_onff > 0) particles[i][j]->render(texture + 4);
							else particles[i][j]->render(texture);
						}
					} else {
						particles[i][j]->render();
					}
				}
			}
		}
	} else {
		glPushMatrix();
		glTranslatef(p_x, p_y, p_z);
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_TEXTURE_2D);
		GLUquadricObj* qobj = gluNewQuadric();
		gluQuadricTexture(qobj, GL_TRUE);
		gluSphere(qobj, radius, 6, 6);
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

inline void Firework::calSpeed(int layer, float angle_lat, float angle_lng, 
	float& speed_x, float& speed_y, float& speed_z)
{
	speed_y = PA_FP_V * sin(angle_lat) * (PA_LAY_MAX - layer);
	speed_x = PA_FP_V * cos(angle_lat) * cos(angle_lng)* (PA_LAY_MAX - layer);
	speed_z = PA_FP_V * cos(angle_lat) * sin(angle_lng)* (PA_LAY_MAX - layer);
}

void Firework::activeParticles()
{
	float angle_lat = PI / 2, angle_lng = 0.0f;
	float speed_x = 0.0f, speed_y = 0.0f, speed_z = 0.0f;
	float a_z = -PA_AIR_RES * GRAVITY;
	for (int m = 0; m < layer; ++m) {
		calSpeed(m, angle_lat, angle_lng, speed_x, speed_y, speed_z);
		particles[m][0]->activate(speed_x, speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		particles[m][1]->activate(speed_x, -speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);

		angle_lat -= PI / 6;
		for (int i = 2; i < 5; ++i) {
			calSpeed(m, angle_lat, angle_lng, speed_x, speed_y, speed_z);
			particles[m][i]->activate(speed_x, speed_y, speed_z,
				wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
			particles[m][i + 3]->activate(speed_x, -speed_y, speed_z,
				wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
			angle_lng += 2 * PI / 3;
		}

		angle_lat -= PI / 6; angle_lng = 0;
		for (int i = 8; i < 13; ++i) {
			calSpeed(m, angle_lat, angle_lng, speed_x, speed_y, speed_z);
			particles[m][i]->activate(speed_x, speed_y, speed_z,
				wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
			particles[m][i + 5]->activate(speed_x, -speed_y, speed_z,
				wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
			angle_lng += 2 * PI / 5;
		}

		angle_lat -= PI / 6; angle_lng = 0;
		for (int i = 18; i < 25; ++i) {
			calSpeed(m, angle_lat, angle_lng, speed_x, speed_y, speed_z);
			particles[m][i]->activate(speed_x, speed_y, speed_z,
				wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
			angle_lng += 2 * PI / 9;
		}
	}
}

void Firework::activeObjModels()
{
	float angle_lat = PI / 4, angle_lng = PI / 4;
	float speed_x = 0.0f, speed_y = 0.0f, speed_z = 0.0f;
	float a_z = - 0.1*GRAVITY;
	for (int m = 0; m < layer; ++m) {
		angle_lng = PI * (rand() % 360) / 180;
		calSpeed(m, angle_lat, angle_lng, speed_x, speed_y, speed_z);
		//speed_y *= 2;
		particles[m][0]->activate(speed_x, speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		particles[m][1]->activate(-speed_x, speed_y, -speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);

		angle_lng = PI * (rand() % 360) / 180;
		calSpeed(m, angle_lat, angle_lng, speed_x, speed_y, speed_z);
		//speed_y *= 2;
		particles[m][2]->activate(speed_x, -speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		particles[m][3]->activate(-speed_x, -speed_y, -speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
	}
}