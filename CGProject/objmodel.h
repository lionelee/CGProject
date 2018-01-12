#ifndef OBJMODEL_H
#define OBJMODEL_H

#include "particle.h"
#include "stdlib.h"
#include "glm.h"

class  ObjModel : public Particle
{
public:
	ObjModel(int type);
	~ObjModel();

	void activate(float speed_x, float speed_y, float speed_z,
		float acc_x, float acc_y, float acc_z, float pos_x, float pos_y, float pos_z);
	void update();
	void render();
	State getStat() { return stat; };

private:
	float origin_y;
	GLMmodel* model_obj;
};

void initObjModel(int type);

#endif // OBJMODEL_H
