#ifndef PARTICLE_H
#define PARTICLE_H

extern float wind_a_x;
extern float wind_a_z;

enum State{								//state of particle
    INIT = 0x0,
    ACTIVE,
    DEAD,
	WAIT
};

class Particle
{
public:
    Particle();
	~Particle() {};

	virtual void init();
	virtual void activate();
	virtual void update();
	virtual void wait();
    virtual void render();
	virtual void activate(float speed_x, float speed_y, float speed_z,
		float acc_x, float acc_y, float acc_z, float pos_x, float pos_y, float pos_z);
	State getStat() { return stat; };
	float getSpeedY() { return v_y; };
	int getUpdateTimes() { return update_times; }

protected:
    float v_x, v_y, v_z;					//speed
    float a_x, a_y, a_z;				//acceleration
    float p_x, p_y, p_z;				//position
    float r, g, b;							//color
    float radius;							//radius
	State stat;							//state
	bool flag;								//using fixed acceleration
	int update_times;					//update times
};

void renderParticles();
void updateParticles();

#endif PARTICLE_H
