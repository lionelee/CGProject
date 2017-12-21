#include "camera.h"
#include "macros.h"
#include <math.h>
#include <stdio.h>

using namespace std;

Camera::Camera() 
{
	cam_x = cam_y = cam_z = 0.0;
	angle_lat = angle_lng = 0.0;
	cen_x = cen_y = cen_z = 0.0;
}

Camera::~Camera() {}

void Camera::init()
{
	cen_x = cam_x = 100.0f;
	cen_y = cam_y = 30.0f;
	cam_z = 160.0f;
	cen_z = cam_z - CAMERA_R;

	face_orient = NORTHEAST;
	flag_left = flag_right = flag_forward = 
		flag_backward = flag_up = flag_down = true;
}

void Camera::look()
{
	gluLookAt(cam_x, cam_y, cam_z, cen_x, cen_y, cen_z, 0.0f, 1.0f, 0.0f);
}

void Camera::look(Direction d)
{
	switch (d){
	case LEFT:
		angle_lng -= CAMERA_A;
		if (angle_lng < -2 * PI) angle_lng = 0;
		break;
	case RIGHT:
		angle_lng += CAMERA_A;
		if (angle_lng > 2 * PI) angle_lng = 0;
		break;
	case UP:
		angle_lat += CAMERA_A;
		angle_lat = MIN(angle_lat, PI/2);
		break;
	case DOWN:
		angle_lat -= CAMERA_A;
		angle_lat = MAX(angle_lat, -PI / 2);
		break;
	default: break;
	}
	checkFacialOrient();

	cen_x = cam_x + CAMERA_R * cos(angle_lat) * sin(angle_lng);
	cen_z = cam_z - CAMERA_R * cos(angle_lat) * cos(angle_lng);
	cen_y = cam_y + CAMERA_R * sin(angle_lat);
	glLoadIdentity();
	gluLookAt(cam_x, cam_y, cam_z, cen_x, cen_y, cen_z, 0.0f, 1.0f, 0.0f);
}

void Camera::move(Direction d)
{
	checkBorderCross();
	double a = CAMERA_V * cos(angle_lng);
	double b = CAMERA_V * sin(angle_lng);
	switch (d){
	case LEFT:
		if (!flag_left) return;
		cam_x -= a; cen_x -= a;
		cam_z -= b; cen_z -= b;
		break;
	case RIGHT:
		if (!flag_right) return;
		cam_x += a; cen_x += a;
		cam_z += b; cen_z += b;
		break;
	case FORWARD:
		if (!flag_forward) return;
		cam_x += b; cen_x += b;
		cam_z -= a; cen_z -= a;
		break;
	case BACKWARD:
		if (!flag_backward) return;
		cam_x -= b; cen_x -= b;
		cam_z += a; cen_z += a;
		break;
	case UP:
		if (!flag_up) return;
		cam_y += CAMERA_V;
		cen_y += CAMERA_V;
		break;
	case DOWN:
		if (!flag_down) return;
		cam_y -= CAMERA_V;
		cen_y -= CAMERA_V;
		break;
	default: break;
	}
	flag_left = flag_right = flag_forward = 
		flag_backward = flag_up = flag_down = true;

	/*cam_x = MAX(cam_x, SCENE_SIZE - TX_SIZE_MIN);
	cam_x = MAX(cam_x, TX_SIZE_MIN);
	cam_z = MAX(cam_z, SCENE_SIZE - TX_SIZE_MIN);
	cam_z = MAX(cam_z, TX_SIZE_MIN);*/
	glLoadIdentity();
	gluLookAt(cam_x, cam_y, cam_z, cen_x, cen_y, cen_z, 0.0f, 1.0f, 0.0f);
}

void Camera::checkFacialOrient()
{
	if ((angle_lng >= 0 && angle_lng <= PI / 2) || (angle_lng <= -3* PI / 2)) {
		face_orient = NORTHEAST;
	} else if ((angle_lng >= -PI / 2 && angle_lng <= 0) || (angle_lng > 3 * PI / 2)){
		face_orient = NORTHWEST;
	} else if ((angle_lng >= PI && angle_lng <= 3 * PI / 2) ||
		(angle_lng >= -PI && angle_lng <= - PI / 2)) {
		face_orient = SOUTHWEST;
	} else {
		face_orient = SOUTHEAST;
	}
}

void Camera::checkBorderCross()
{
	if (face_orient == NORTHEAST) {
		if (cam_x < GRID_SIZE) flag_left = flag_backward = false;
		if (cam_z < GRID_SIZE) flag_left = flag_forward = false;
		if (cam_x > SCENE_SIZE - GRID_SIZE) flag_right = flag_forward = false;
		if (cam_z > SCENE_SIZE - GRID_SIZE) flag_right = flag_backward = false;
	} else if (face_orient == NORTHWEST) {
		if (cam_x < GRID_SIZE) flag_left = flag_forward = false;
		if (cam_z < GRID_SIZE) flag_right = flag_forward = false;
		if (cam_x > SCENE_SIZE - GRID_SIZE) flag_right = flag_backward = false;
		if (cam_z > SCENE_SIZE - GRID_SIZE) flag_left = flag_backward = false;
	}else if (face_orient == SOUTHWEST) {
		if (cam_x < GRID_SIZE) flag_right = flag_forward = false;
		if (cam_z < GRID_SIZE) flag_right = flag_backward = false;
		if (cam_x > SCENE_SIZE - GRID_SIZE) flag_left = flag_backward = false;
		if (cam_z > SCENE_SIZE - GRID_SIZE) flag_left = flag_forward = false;
	}else if (face_orient == SOUTHEAST) {
		if (cam_x < GRID_SIZE) flag_right = flag_backward = false;
		if (cam_z < GRID_SIZE) flag_left = flag_backward = false;
		if (cam_x > SCENE_SIZE - GRID_SIZE) flag_left = flag_forward = false;
		if (cam_z > SCENE_SIZE - GRID_SIZE) flag_right = flag_forward = false;
	}
	//if (cam_y > SCENE_HT - GRID_SIZE) flag_up = false;
	else if (cam_y < GRID_SIZE) flag_down = false;
}