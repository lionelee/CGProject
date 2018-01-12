#ifndef LIGHT_H
#define LIGHT_H

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/GLUT.H>

enum LightDirec {
	LLEFT = 0,
	LRIGHT,
	LUP,
	LDOWN,
	LFORWARD,
	LBACKWARD
};

class Light
{
public:
	Light();
	~Light() {};

	void illuminate();
	void move(LightDirec direc);
	void spot(LightDirec direc);
	void update();
	GLenum accquireLightSrc();
	void releaseLightSrc(GLenum light);

private:
	double angle_lat, angle_lng;					//light rotating 
	float spotDirec[4] = { 0.0f, -1.0f, -0.0f, 0.0f };
	GLenum light_src[7] = { GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };
};

#endif // LIGHT_H
