#ifndef GUI_H
#define GUI_H

#include <GL/glui.h>

#define	SPN_FBOX_X			1
#define	SPN_FBOX_Z			2
#define	BTN_ADD_FBOX		3
#define	BTN_DEL_FBOX		4
#define	BTN_ADD_TREE		5
#define	BTN_DEL_TREE		6

class GUI 
{
public:
	void init(int win_id);

//private:
	/*controls*/
	GLUI*							glui1;
	GLUI_Panel*				panel_coefficient;
	GLUI_Panel*				panel_light;
	GLUI_RadioGroup*		ligroup;

	GLUI*							glui2;
	GLUI_Panel*				panel_firework;
	GLUI_RadioGroup*		fbgroup;
	GLUI_Panel*				panel_tree;
	void a(int);
};

void controlLight(int control);
void controlFirework(int control);
void controlTree(int control);

#endif GUI_H
