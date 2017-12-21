#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

enum Direction {				//camera direction
	LEFT = 0x10,
	RIGHT,
	UP,
	DOWN,
	FORWARD,
	BACKWARD,
	NORTHEAST,
	NORTHWEST,
	SOUTHWEST,
	SOUTHEAST
};

class Camera
{
public:
	Camera();
	~Camera();

	void init();
	void look();
	void look(Direction d);
	void move(Direction d);

private:
	GLdouble cam_x, cam_y, cam_z;				//camera position
	GLdouble cen_x, cen_y, cen_z;					//camera eyeing-at position
	double angle_lat, angle_lng;					//camera rotating angle

	Direction face_orient;								//facial orientation
	bool flag_left, flag_right, flag_forward,
		flag_backward, flag_up, flag_down;	//flag to prevent beyond border
	
	void checkFacialOrient();							//check facial orientation
	void checkBorderCross();						//check whether crossing border
};

#endif CAMERA_H