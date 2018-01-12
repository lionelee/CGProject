#ifndef FIREWORK_H
#define FIREWORK_H

#include "particle.h"

class Firework : Particle
{
public:
	Firework(int posx, int posz, int tx, int _layer, int _type, unsigned int light);
	~Firework();

	void init();
	void activate();
	void update();
	void wait();
	void render();
	State getStat() { return stat; };

private: 
	Particle*** particles;									//sub particles within the firework particle
	int texture;													//texture of the firework particle and subs
	bool flag_explosion;									//exploded 
	int layer;														//number of subparticles' layer
	int subtotal;												//number of each layer's sub particles
	int type;														//type of subparticle
	int num;														//number of sub particles
	int dead_particle;										//number of dead sub particles
	State stat;													//state
	int bkg_times;												//background shinning time
	unsigned int bkg_amb;								//background ambient light
	float bkg[4];												//attribute background ambient light

	inline void calSpeed(int layer,					//calculate speed of sub particle 
		float angle_lat, float angle_lng, float& speed_x, float& speed_y, float& speed_z);
	void activeParticles();									//active each layer's sub particles
	void activeObjModels();								//active sub particles with obj models
};

#endif FIREWORK_H
