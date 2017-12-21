#ifndef  FIREWOEKBOX_H
#define FIREWOEKBOX_H

#include "firework.h"

class Fireworkbox
{
public:
	Fireworkbox(float p_x, float p_z);
	~Fireworkbox();

	void render();
	void update();

private:
	float pos_x, pos_z;									//firework box position(on ground)
	Firework** fireworks;								//firework particles within the firework box
	int dead_firework;									//number of dead fireworks
	int flag_emit;											//flag to emit a new firework particle
};

void renderFireworkboxs();
void updateFireworkboxs();

#endif  FIREWOEKBOX_H