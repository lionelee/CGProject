#ifndef  FIREWOEKBOX_H
#define FIREWOEKBOX_H

#include "firework.h"

class Fireworkbox
{
public:
	Fireworkbox(int p_x, int p_z, int _layer, int _type);
	~Fireworkbox();

	void renderBox();
	void render();
	void update();
	int getPosx() { return pos_x; }
	int getPosz() { return pos_z; }

private:
	int pos_x, pos_z;										//firework box position(on ground)
	int layer;													//number of subparticles' layer
	int type;													//type of subparticle
	Firework** fireworks;								//firework particles within the firework box
	int dead_firework;									//number of dead fireworks
	int flag_emit;											//flag to emit a new firework particle
	unsigned int amb;									//background ambient light

	float mat[3][4] = {
		{ 0.60f, 0.60f, 0.60f, 1.0f },
		{ 0.75f, 0.75f, 0.75f, 1.0f },
		{ 0.89f, 0.89f, 0.89f, 1.0f }
	};
};

void addFireworkbox(int p_x, int p_z, int _layer, int _type);
void removeFireworkbox(int p_x, int p_z);
void renderFireworkboxes();
void updateFireworkboxes();

#endif  FIREWOEKBOX_H