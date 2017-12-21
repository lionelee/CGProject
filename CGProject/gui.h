#ifndef GUI_H
#define GUI_H

#include <GL/glui.h>

#define	SPN_GRAVITY	1
#define	SPN_WIND_X	2
#define	SPN_WIND_Z	3
#define	CHK_ENDLESS 4

class GUI 
{
public:
	void init(int win_id);

private:
	GLUI* glui;
	GLUI_Panel* panel_coefficient;
	GLUI_Panel* panel_light;
	GLUI_Panel* panel_firework;
	GLUI_Panel* panel_tree;

	void paramter_cb(int control);
	void control();
	void adjustLight();
	void adjustLightPos();

};

#endif GUI_H
