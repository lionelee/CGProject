#ifndef TREE_H
#define TREE_H

#include <stdlib.h>
#include <GL/glut.h>
#include <queue>
#include "leave.h"

class Tree 
{
public:
	Tree(int posx, int posz,int level, int tx, float ratio, float angle, float height);
	~Tree();

	void update();
	void render();
	int getPosx() { return pos_x; }
	int getPosz() { return pos_z; }

private:
	int pos_x, pos_z;										//tree position(on ground)
	float height, max_height;						//base height of trunk
	float radius, max_radius;						//base radius of trunk
	int level;													//recursive level
	int texture;												//texture of trunk
	float ratio;												//height of parent branch devided by sub branch
	float angle, max_angle;							//inclination
	int update_times;									//update times
	int fade_leaves;										//faded leaves number
	int isolate_leaves;									//isolated leaves number
	int dead_leaves;										//dead leaves number
	GLUquadricObj* qobj;							//quadric obj to draw trunk
	std::queue<Leave*> leaves;					//queue of leaves
	std::queue<Leave*> iso_leaves;			//queue of isolated leaves
	char* random;										//array to record random number
	float tree_mat[3][4] = {
		{ 0.96f, 0.96f, 0.96f, 1.0f },
		{ 0.97f, 0.97f, 0.97f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f }
	};

	void render_branch(int l, float h, float r);
	void calculate_pos(float posx, float posy, float posz, float h, float a, int l);
};

void addTree(int p_x, int p_z, int level, float ratio, float angle, float height);
void removeTree(int p_x, int p_z);
void renderTrees();
void updateTrees();

#endif // !TREE_H