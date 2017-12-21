#include "firework.h"
#include "macros.h"
#include <vector>
#include <iostream>
#include <GL/glut.h>

using namespace std;

//layer of sub particles within a firework particle
int layers = 1;
//number of each layer's sub particles
int subtotal = 25;

//vector of firework boxs
vector<Firework*> fbox;

Firework::Firework(int posx, int posz)
{
	p_x = posx;
	p_z = posz;
	p_y = FBOX_SIZE;
	a_z = -FP_AIR_RES * GRAVITY;
	stat = INIT;
}

Firework::~Firework()
{
	if (particles != NULL) {
		for (int i = 0; i < layers; ++i) {
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
	a_y = -GRAVITY;
	v_x = v_z = 0.0f; v_y = FP_V;
	radius = 0.5;
	dead_particle = flag_emit = 0;
	num = layers * subtotal;
	if (particles == NULL) {
		particles = new Particle**[layers];
		for (int i = 0; i < layers; ++i) {
			particles[i] = new Particle*[subtotal];
			for (int j = 0; j < subtotal; ++j)
				particles[i][j] = new Particle();
		}
	}
}

void Firework::update()
{
	if (dead_particle == layers * subtotal) {
		stat = DEAD;
		return;
	}
	if (v_y <= 0) {									//explosion
		if (flag_emit % 16 == 0) {
			int cur_layer = flag_emit / 16;
			if(cur_layer < layers) activeParticles(cur_layer);
		}
		for (int i = 0; i < layers; ++i) {
			for (int j = 0; j < subtotal; ++j) {
				if (particles[i][j] == NULL) continue;
				if (particles[i][j]->getStat() == ACTIVE) {
					if (particles[i][j]->getUpdateTimes() > 5) {
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
		++flag_emit;
	} else {
		v_y += a_y;
		p_y += v_y;
	}
}

void Firework::wait()
{
	stat = WAIT;
	for (int i = 0; i < layers; ++i) {
		for (int j = 0; j < subtotal; ++j)
			particles[i][j]->init();
	}
}

void Firework::render()
{
	if (v_y <= 0) {									//explosion
		for (int i = 0; i < layers; ++i) {
			for (int j = 0; j < subtotal; ++j) {
				if (particles[i][j] != NULL && particles[i][j]->getStat() == ACTIVE) {
					particles[i][j]->render();
				}
			}
		}
	} else {
		glPushMatrix();
		glTranslatef(p_x, p_y, p_z);
		glutSolidSphere(radius, 6, 6);
		glPopMatrix();
	}
}

inline void Firework::calSpeed(float angle_lat, float angle_lng, 
	float& speed_x, float& speed_y, float& speed_z)
{
	speed_y = PA_FP_V * sin(angle_lat);
	speed_x = PA_FP_V * cos(angle_lat) * cos(angle_lng);
	speed_z = PA_FP_V * cos(angle_lat) * sin(angle_lng);
}

void Firework::activeParticles(int _layer)
{
	float angle_lat = PI / 2, angle_lng = 0.0f;
	float speed_x = 0.0f, speed_y = 0.0f, speed_z = 0.0f;
	float a_z = -PA_AIR_RES*GRAVITY;

	calSpeed(angle_lat, angle_lng, speed_x, speed_y, speed_z);
	particles[_layer][0]->activate(speed_x, speed_y, speed_z,
		wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
	particles[_layer][1]->activate(speed_x, -speed_y, speed_z,
		wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
	
	angle_lat -= PI / 6;
	for (int i = 2; i < 5; ++i) {
		calSpeed(angle_lat, angle_lng, speed_x, speed_y, speed_z);
		particles[_layer][i]->activate(speed_x, speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		particles[_layer][i+3]->activate(speed_x, -speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		angle_lng += 2 * PI / 3;
	}

	angle_lat -= PI / 6; angle_lng = 0;
	for (int i = 8; i < 13; ++i) {
		calSpeed(angle_lat, angle_lng, speed_x, speed_y, speed_z);
		particles[_layer][i]->activate(speed_x, speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		particles[_layer][i + 5]->activate(speed_x, -speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		angle_lng += 2 * PI / 5;
	}

	angle_lat -= PI / 6; angle_lng = 0;
	for (int i = 18; i < 25; ++i) {
		calSpeed(angle_lat, angle_lng, speed_x, speed_y, speed_z);
		particles[_layer][i]->activate(speed_x, speed_y, speed_z,
			wind_a_x, a_z, wind_a_z, p_x, p_y, p_z);
		angle_lng += 2 * PI / 9;
	}
}