#ifndef SCENE_H
#define SCENE_H

#include "macros.h"

class Scene
{
public:
    Scene() {}
    ~Scene() {}

	void init();
	void render();

private:
	unsigned int texture[TX_NUM];

	void renderSkybox();
};

#endif SCENE_H
