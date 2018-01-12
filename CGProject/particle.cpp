#include "particle.h"
#include "macros.h"
#include <stdlib.h>
#include <stdio.h>
#include "camera.h"
#include "glm.h"
#include <map>

//number of active particle
int active_num = 0;
//speed of newly actived particle
int active_speed = 6;

//coefficient of acceleration of wind
float wind_a_x = 0.0f;
float wind_a_z = 0.0f;

extern int light_onff;
extern std::map<int, GLMmodel*> objs;
extern Camera camera;

Particle::Particle()
{
    stat = INIT;
    v_x = v_y = v_z = 0.0f;
    a_x = a_y = a_z = 0.0f;
    p_x = p_y = p_z = 0.0f;
    r = g = b = 0.0f;
    radius = 0.0f;
}

void Particle::init()
{
	stat = INIT;
	v_x = v_y = v_z = 0.0f;
	a_x = a_y = a_z = 0.0f;
	p_x = p_y = p_z = 0.0f;
	r = g = b = 0.0f;
	radius = 0.0f;
}

void Particle::activate()
{
    stat = ACTIVE;
	v_x = v_y = v_z = 0.0f;
	a_x = wind_a_x; a_z = wind_a_z; a_y = -0.01 * GRAVITY;

    p_y = ACTIVE_Y;
    p_x = float(rand()) * SCENE_SIZE / RAN_MAX;
    p_z = float(rand()) * SCENE_SIZE / RAN_MAX;

    r = g = b = 1.0f;

    radius = float(rand()) *0.1 / RAN_MAX + 0.2;
	update_times = 0;
}

void Particle::activate(float speed_x, float speed_y, float speed_z,
	float acc_x, float acc_y, float acc_z, float pos_x, float pos_y, float pos_z)
{
	stat = ACTIVE;
	v_x = speed_x; v_y = speed_y; v_z = speed_z;
	a_x = acc_x; a_y = acc_y; a_z = acc_z;
	p_x = pos_x; p_y = pos_y; p_z = pos_z;

	r = g = b = 1.0f;	radius = 1.5;
	update_times = 0;
	flag = true;
}

void Particle::update()
{
	if (stat == DEAD)return;
	//when out of border
	if (p_y <= 0 || p_x <= 0 || p_x >= SCENE_SIZE || p_z <= 0 || p_z >= SCENE_SIZE) {
		stat = DEAD;
		return;
	}
	++update_times;
	if (!flag) {
		a_x = wind_a_x;
		a_z = wind_a_z;
	}

    v_x += a_x;
    v_y += a_y;
    v_z += a_z;
    p_x += v_x;
    p_y += v_y;
    p_z += v_z;
}

void Particle::wait()
{
	stat = WAIT;
}

void Particle::render()
{
    glPushMatrix();
    glTranslatef(p_x, p_y, p_z);
    glutSolidSphere(radius, 6, 6);
    glPopMatrix();
}

void Particle::render(int texture)
{
	glPushMatrix();
	glTranslatef(p_x, p_y, p_z);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	if (light_onff == 0) {
		GLUquadricObj* qobj = gluNewQuadric();
		gluQuadricNormals(qobj, GLU_SMOOTH);
		gluQuadricTexture(qobj, GL_TRUE);
		gluSphere(qobj, 0.6f, 6, 6);
	}else {
		/*glColor4ub(0, 0, 0, 255);
		glutSolidCube(radius);*/

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		float lng = -camera.getAngleLng() * 360 / PI;
		glRotatef(lng, 0, 1, 0);
		float x = cos(camera.getAngleLng()), z = sin(camera.getAngleLng());
		float lat = camera.getAngleLat() * 360 / PI;
		glRotatef(lat, x, 0, z);
		
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
		glTexCoord2f(1, 0); glVertex3f(radius, 0, 0);
		glTexCoord2f(1, 1); glVertex3f(radius, radius, 0);
		glTexCoord2f(0, 1); glVertex3f(0, radius, 0);
		glEnd();

		/*float k = radius / 2.0;
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-k, -k, -k);
		glTexCoord2f(1, 0); glVertex3f(-k, -k, k);
		glTexCoord2f(1, 1); glVertex3f(-k, k, k);
		glTexCoord2f(0, 1); glVertex3f(-k, k, -k);
		glEnd();

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-k, -k, k);
		glTexCoord2f(1, 0); glVertex3f(k, -k, k);
		glTexCoord2f(1, 1); glVertex3f(k, k, k);
		glTexCoord2f(0, 1); glVertex3f(-k, k, k);
		glEnd();

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(k, -k, k);
		glTexCoord2f(1, 0); glVertex3f(k, -k, -k);
		glTexCoord2f(1, 1); glVertex3f(k, k, -k);
		glTexCoord2f(0, 1); glVertex3f(k, k, k);
		glEnd();

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(k, -k, -k);
		glTexCoord2f(1, 0); glVertex3f(-k, -k, -k);
		glTexCoord2f(1, 1); glVertex3f(-k, k, -k);
		glTexCoord2f(0, 1); glVertex3f(k, k, -k);
		glEnd();*/

		glColor3f(1.0, 1.0, 1.0);
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Particle::renderObj()
{
	GLMmodel* model_obj = objs[1];
	glPushMatrix();
	glTranslatef(p_x, p_y, p_z);
	glmUnitize(model_obj);
	glmScale(model_obj, 4.0f);
	glmDraw(model_obj, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}

Particle* particle = new Particle[PAR_MAX];

void renderParticles()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glDisable(GL_LIGHTING);
	for (int i = 0; i < PAR_MAX; ++i) {
		if (particle[i].getStat() != ACTIVE)
			continue;
		particle[i].render();
	}
	glEnable(GL_LIGHTING);
}

void updateParticles()
{
	int new_active = 0;
	for (int i = 0; i < PAR_MAX; ++i) {
		if (particle[i].getStat()== ACTIVE) {
			particle[i].update();
		} else {
			if (new_active < active_speed) {			//re-active dead particle 
				particle[i].activate();
				++new_active;
				++active_num;
			}
			continue;
		}
	}
}



