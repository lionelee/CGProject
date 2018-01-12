#ifndef LEAVE_H
#define LEAVE_H

#include "particle.h"
#include "macros.h"

class Leave: Particle
{
public:
	Leave(float posx, float posy, float posz, float scale);
	Leave();
	~Leave() {};

	void activate(float posx, float posy, float posz);
	void reactive();
	void update();
	void render_isolate();
	void render();
	State getStat() { return stat; };
	bool setFade();
	void setIsolate();
	void wait() { stat = WAIT; };

private:
	float init_x, init_y, init_z;
	State stat;
	unsigned char r, g, b;
	int range = 2 * GRID_SIZE;
	float ratio;
	float angle;
	int scale;
	bool flag_grow;
	float mat[3][4];
};

#endif // LEAVE_H