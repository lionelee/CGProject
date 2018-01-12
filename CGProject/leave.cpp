#include "leave.h"
#include <stdlib.h>
#include <GL/glut.h>

Leave::Leave(float posx, float posy, float posz, float _scale)
	:init_x(posx), init_y(posy), init_z(posz)
{
	stat = ACTIVE;
	r = 0; g = 255; b = 0;	
	ratio = 0.15f;
	scale = 1.9*_scale;
	flag_grow = true;
	p_x = posx;	p_y = posy;	p_z = posz;
	mat[0][0] = 0.1216; mat[0][1] = 0.6666; mat[0][2] = 0; mat[0][3] = 1.0;
	mat[1][0] = 0.2921; mat[1][1] = 0.8666; mat[1][2] = 0.0902; mat[1][3] = 1.0;
	mat[2][0] = 0.2517; mat[2][1] = 0.8906; mat[2][2] = 0.0517; mat[2][3] = 1.0;
}

Leave::Leave()
{
	stat = INIT;
	r = 0; g = 255; b = 0;
	ratio = 0.15f;
	scale = -1;
	flag_grow = true;
	mat[0][0] = 0.1216; mat[0][1] = 0.6666; mat[0][2] = 0; mat[0][3] = 1.0;
	mat[1][0] = 0.2921; mat[1][1] = 0.8666; mat[1][2] = 0.0902; mat[1][3] = 1.0;
	mat[2][0] = 0.2517; mat[2][1] = 0.8906; mat[2][2] = 0.0517; mat[2][3] = 1.0;
}

void Leave::activate(float posx, float posy, float posz)
{
	stat = ACTIVE;
	init_x = p_x = posx;
	init_y = p_y = posy + 0.5*(rand() % 10 - 5);
	init_z = p_z = posz;
}

void Leave::reactive()
{
	stat = ACTIVE;
	ratio = 0.15f;
	scale = -1;
	flag_grow = true;
	p_x = init_x; p_y = init_y; p_z = init_z;
	mat[0][0] = 0.1216; mat[0][1] = 0.6666; mat[0][2] = 0; mat[0][3] = 1.0;
	mat[1][0] = 0.2921; mat[1][1] = 0.8666; mat[1][2] = 0.0902; mat[1][3] = 1.0;
	mat[2][0] = 0.2517; mat[2][1] = 0.8906; mat[2][2] = 0.0517; mat[2][3] = 1.0;
}

void Leave::update()
{
	if (stat != ISOLATE)return;
	if (p_y <= 0 || p_x <= 0 || p_x >= SCENE_SIZE || p_z <= 0 || p_z >= SCENE_SIZE) {
		stat = DEAD;
		return;
	}
	a_x = wind_a_x;
	a_z = wind_a_z;
	float a = -LEA_AIR_RES * v_y;
	v_x += a_x;
	v_y += a_y + a;
	v_z += a_z;
	p_x += v_x;
	p_y += v_y;
	p_z += v_z;
}

void Leave::render_isolate()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat[2]);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

	glPushMatrix();
	glTranslatef(p_x, p_y, p_z);
	angle += 5;
	float m = 1.0f*ratio;
	glTranslatef(m/2, 0, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(-m/2, 0, 0);
	GLfloat ctrlPoints[3][3][3] = {
		{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, m },{ m, m, m } },
	{ { 0.0f, 0.0f, 0.0f },{ 0.0f, m, 0.0f },{ m, m, m } },
	{ { 0.0f, 0.0f, 0.0f },{ m, 0.0f, 0.0f },{ m, m, m } }
	};
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 10.0f, 3, 3, 0.0f, 10.0f, 9, 3, &ctrlPoints[0][0][0]);
	glMapGrid2f(10, 0.0f, 10.0f, 10, 0.0f, 10.0f);
	glEvalMesh2(GL_FILL, 0, 10, 0, 10);
	glPopMatrix();
}

void Leave::render()
{
	if (ratio < 1.6f)ratio *= 1.008;
	else flag_grow = false;
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat[1]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat[2]);
	glMaterialf(GL_FRONT, GL_SHININESS, 180.0);

	float m = 1.0f*ratio;
	GLfloat ctrlPoints[3][3][3] = {
		{{0.0f, 0.0f, 0.0f},{0.0f, 0.0f, m},{m, m, m}},
		{{0.0f, 0.0f, 0.0f},{0.0f, m, 0.0f},{m, m, m}},
		{{0.0f, 0.0f, 0.0f},{m, 0.0f, 0.0f},{m, m, m}}
	};
	glEnable(GL_MAP2_VERTEX_3);
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 10.0f, 3, 3, 0.0f, 10.0f, 9, 3, &ctrlPoints[0][0][0]);
	glMapGrid2f(10, 0.0f, 10.0f, 10, 0.0f, 10.0f);
	glEvalMesh2(GL_FILL, 0, 10, 0, 10);
}

bool Leave::setFade()
{
	if (flag_grow)return false;
	stat = FADE;
	r = 82; g = 187; b = 3;
	mat[0][0] = 0.5498; mat[0][1] = 0.5666; mat[0][2] = 0.0902; mat[0][3] = 1.0;
	mat[1][0] = 0.5896; mat[1][1] = 0.7157; mat[1][2] = 0.0712; mat[1][3] = 1.0;
	mat[2][0] = 0.6263; mat[2][1] = 0.7959; mat[2][2] = 0.0686; mat[2][3] = 1.0;
	return true;
}

void Leave::setIsolate()
{
	stat = ISOLATE;
	a_y = -0.001 * GRAVITY;
	r = 82; g = 77; b = 1;
	mat[0][0] = 0.9007; mat[0][1] = 0.5980; mat[0][2] = 0.0902; mat[0][3] = 1.0;
	mat[1][0] = 0.8512; mat[1][1] = 0.6275; mat[1][2] = 0.0; mat[1][3] = 1.0;
	mat[2][0] = 0.8064; mat[2][1] = 0.6588; mat[2][2] = 0.0145; mat[2][3] = 1.0;
	angle = 0;
	if (scale > -1) {
		p_x = p_x + rand() % scale - scale / 2;
		p_z = p_z + rand() % scale - scale / 2;
	}
}