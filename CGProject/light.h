#ifndef LIGHT_H
#define LIGHT_H

class Light
{
public:
	Light();
	~Light() {};

	void init();
	void adjustPos();
	void adjustAmbient();
	void adjustDiffuse();
	void adjustSpecular();
};

#endif // LIGHT_H
