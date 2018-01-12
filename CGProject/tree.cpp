#include "tree.h"
#include "macros.h"
#include <vector>
#include <iostream>

using namespace std;

extern unsigned int texture[TX_NUM];

Tree::Tree(int posx, int posz, int _level, int tx, float _ratio, float _angle, float _height)
	:pos_x(posx), pos_z(posz), level(_level),texture(tx), 
	ratio(_ratio), max_angle(_angle), max_height(_height)
{
	height = 3.0f;
	angle = 10.0f;
	radius = 0.3f;
	max_radius = 0.3*level;
	update_times = isolate_leaves = fade_leaves = 0;
	qobj = gluNewQuadric();

	int num = 2 * pow(3, level);
	random = new char[num];
	for (int i = 0; i < num; ++i) random[i] = 0;
	float h = max_height, tmp = max_height*ratio;
	float w = 0;
	for (int i = 1; i < level; ++i) {
		tmp *= ratio;
		float a = i * max_angle * PI / 180;
		h += tmp * cos(a);
		w += tmp * sin(a);
	}

	while (num-- > 0) {
		//leaves.push(new Leave(pos_x, y, pos_z, w));
		leaves.push(new Leave());
	}
	calculate_pos(pos_x, h, pos_z, max_height, max_angle, 0);
}

Tree::~Tree()
{
	while (!leaves.empty()) {
		Leave* l = leaves.front(); leaves.pop();
		delete l;
	}
	delete[] random;
}

void Tree::update()
{
	int size = iso_leaves.size();
	while (size-- > 0) {
		Leave* l = iso_leaves.front(); iso_leaves.pop();
		if (l->getStat() == ISOLATE) {
			l->update(); iso_leaves.push(l);
		}
	}
	size = leaves.size();
	if (dead_leaves >= size) {
		if (update_times % REGW_INT == 0) {
			int m = size;
			cout << "m:" << m << "\tsize:" << leaves.size() << endl;
			while (m-- > 0) {
				Leave* leave = leaves.front(); leaves.pop();
				if (leave->getStat() == WAIT) {
					leave->reactive();
				}
				leaves.push(leave);
			}
			for (int i = 0; i < size; ++i) random[i] = 0;
			update_times = dead_leaves = fade_leaves = isolate_leaves = 0;
		}
		return;
	}
	if (height <= max_height) {
		radius += (max_radius - 0.3f) / GROW_INT;
		height += (max_height - 3.0f) / GROW_INT;
		angle += (max_angle - 10.0f) / GROW_INT;
	} else if (fade_leaves < size) {
		if (update_times % FADE_INT == 0) {
			int i = rand() % size, res = size-1 - i;
			if (random[i] > 0) {
				++update_times;
				return;
			}
			Leave* le;
			le = leaves.front(); leaves.pop();
			while (i-- > 0) {
				leaves.push(le);
				le = leaves.front(); leaves.pop();
			}
			if (le->getStat() == ACTIVE &&le->setFade()){
				++fade_leaves;
				random[i] = 1;
				if (fade_leaves == size) {
					for (int i = 0; i < size; ++i)random[i] = 0;
				}
			}
			leaves.push(le);
			while (res-- > 0) {
				le = leaves.front(); 
				leaves.pop();
				leaves.push(le);
			}
			update_times = 0;
		}
	} else if(isolate_leaves <= size){
		if (isolate_leaves == size)++isolate_leaves;
		if (update_times % LEAVE_INT == 0 && isolate_leaves  <size) {
			int i = rand() % size, res = size - 1 - i;
			if (random[i] > 0) {
				++update_times;
				return;
			}
			random[i] = 1;
			Leave* le;
			le = leaves.front(); leaves.pop();
			while (i-- > 0) {
				leaves.push(le);
				le = leaves.front(); leaves.pop();
			}
			if (le->getStat() == FADE) {
				le->setIsolate();
				++isolate_leaves;
				iso_leaves.push(le);
			}
			leaves.push(le);
			while (res-- > 0) {
				le = leaves.front();
				leaves.pop();
				leaves.push(le);
			}
			update_times = 0;
		}
	} else {
		int s = size;
		while (s-- > 0) {
			Leave* leave = leaves.front(); leaves.pop();
			if (leave->getStat() == DEAD) {
				++dead_leaves;
				leave->wait();
			}
			leaves.push(leave);
		}
	}
	++update_times;
}

void Tree::render()
{
	glEnable(GL_LIGHTING);
	glPushMatrix(); 
	glTranslatef(pos_x, 0, pos_z);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricTexture(qobj, GL_TRUE);
	render_branch(0, height, radius);
	glPopMatrix();
	int size = iso_leaves.size();
	while (size-- > 0) {
		Leave* le = iso_leaves.front(); iso_leaves.pop();
		le->render_isolate();	iso_leaves.push(le);
	}
}

void Tree::render_branch(int l, float h, float r)
{
	if (l > level)	return;
	h *= ratio; r *= 0.5;

	glMaterialfv(GL_FRONT, GL_AMBIENT, tree_mat[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, tree_mat[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, tree_mat[2]);
	glMaterialf(GL_FRONT, GL_SHININESS, 150.0);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	gluCylinder(qobj, r, 0.6*r, h, TRK_SLICES/(l+1), TRK_STACKS/(l+1));
	glDisable(GL_TEXTURE_2D);
	
	if (l == level && !leaves.empty()) {			//draw leaves
		glTranslatef(0, 0, h);
		Leave* le = leaves.front(); leaves.pop();
		if (le->getStat() == ACTIVE || le->getStat() == FADE) {
			le->render();
		}
		leaves.push(le);
		glRotatef(180, 0, 0, 1);
		le = leaves.front(); leaves.pop();
		if (le->getStat() == ACTIVE || le->getStat() == FADE) {
			le->render();
		}
		leaves.push(le);
		
	}
	{
		/*glPushMatrix();
		glTranslatef(0, 0, h);
		render_branch(l + 1, h, r);
		glPopMatrix();*/

		glTranslatef(0, 0, h);
		for (int i = 0; i < 3; ++i) {
			glPushMatrix();
			glRotatef(i * 120 + 12*l, 0, 0, 1);
			glRotatef(-angle, 0, 1, 0);
			render_branch(l + 1, h, r);
			glPopMatrix();
		}
	}
}

/*  (1)branch1
            \___
		    /\
			   \______\____ (0)branch0
			   /         / 
		    \/__
		   /
	(2)branch2
*/
//approximate position of leaves
void Tree::calculate_pos(float posx, float posy, float posz, float h, float a, int l)
{
	if (l > level)return;
	/*float px = posx, py = posy + h, pz = posz;
	if (l == level) {
		Leave* le = leaves.front(); leaves.pop();
		le->activate(px, py, pz);
	}
	else {
		calculate_pos(px, py, pz, h*ratio, a+angle,l + 1);
	}*/

	float sina = sin(a), cosa = cos(a), py = posy;
	float px = posx + h*sina, pz = posz;												//(0)
	if (l == level) {
		Leave* le = leaves.front(); leaves.pop();
		le->activate(px, py, pz);  leaves.push(le);
	} else {
		calculate_pos(px, py, pz, h*ratio, a + angle, l + 1);
	}

	float iz = h * sina*0.866;
	px = posx - h*sina*0.5, pz = posz - iz;											//(1)
	if (l == level) {
		Leave* le = leaves.front(); leaves.pop();
		le->activate(px, py, pz); leaves.push(le);
	} else {
		calculate_pos(px, py, pz, h*ratio, a + angle, l + 1);
	}

	pz = posz + iz;																				//(2)
	if (l == level) {
		Leave* le = leaves.front(); leaves.pop();
		le->activate(px, py, pz); leaves.push(le);
	} else {
		calculate_pos(px, py, pz, h*ratio, a + angle, l + 1);
	}
}


vector<Tree*> trees;
void addTree(int p_x, int p_z, int level, float ratio, float angle, float height)
{
	trees.push_back(new Tree(p_x, p_z, level, texture[15], ratio, angle, height));
}

void removeTree(int p_x, int p_z)
{
	vector<Tree*>::iterator it = trees.begin();
	for (; it != trees.end(); ++it) {
		if ((*it)->getPosx() == p_x && (*it)->getPosz() == p_z) {
			delete (*it);
			trees.erase(it);
			return;
		}
	}
}

void renderTrees()
{
	int size = trees.size();
	for (int i = 0; i < size; ++i) {
		trees[i]->render();
	}
}

void updateTrees()
{
	int size = trees.size();
	for (int i = 0; i < size; ++i) {
		trees[i]->update();
	}
}