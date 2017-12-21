#ifndef FIREWORK_H
#define FIREWORK_H

#include "particle.h"

class Firework : Particle
{
public:
	Firework(int posx, int posz);
	~Firework();

	void init();
	void activate();
	void update();
	void wait();
	void render();
	State getStat() { return stat; };

private: 
	Particle*** particles;									//sub particles within the firework particle
	int num;													//number of sub particles
	int dead_particle;										//number of dead sub particles
	int flag_emit;											//flag to emit a new layer's sub particles
	State stat;												//state

	inline void calSpeed(float angle_lat,		//calculate speed of sub particle
		float angle_lng, float& speed_x, float& speed_y, float& speed_z);
	void activeParticles(int _layer);				//active each layer's sub particles
};

#endif FIREWORK_H
