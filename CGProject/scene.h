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
	bool occupy(int x, int z, int type);
	bool release(int x, int z, int type);

private:
	void renderSkybox();
	int grid[SCENE_SIZE / GRID_SIZE][SCENE_SIZE / GRID_SIZE];

	float mat[3][4] = {
		{ 0.75f, 0.75f, 0.75f, 1.0f },
		{ 0.8f, 0.8f, 0.8f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }
	};
};

#endif SCENE_H
