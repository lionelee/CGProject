#include "light.h"
#include "macros.h"
#include <math.h>

int light_onff = 0, light_mode = 0;
float cutoff = 16.0f, exponent = 5.0f;
float const_atten = 1.0f, linear_atten = 0.0f, quadr_atten = 0.0f;

float ambientLight[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float diffuseLight[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
float specularLight[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightPos[4] = { 100.0f, 50.0f, 100.0f, 1.0f };
float amb[4] = { 0.08f, 0.08f, 0.08f, 1.0f };

Light::Light()
{
	angle_lat = angle_lng = 0.0;
}

void Light::illuminate()
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	glEnable(GL_LIGHTING);
	/*float pos[4] = { 120,120, 25, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, pos);
	float atten = 1.0f;
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, atten);
	float bkg[4] = { 0.08f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, bkg);
	glEnable(GL_LIGHT1);*/
	if (light_onff > 0) return;
	else {
		if (light_mode == POINT_LIGHT || light_mode == SPOT_LIGHT)
			lightPos[3] = 1.0f;
		if (light_mode == PARAL_LIGHT)lightPos[3] = 0.0f;
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, const_atten);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linear_atten);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadr_atten);
		if (light_mode == SPOT_LIGHT) {
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cutoff);
			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirec);
			glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, exponent);
		}
		else {
			glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
		}
		glEnable(GL_LIGHT0);
	}
}

void Light::move(LightDirec direc)
{
	if (light_onff > 0)return;
	switch (direc) {
	case LLEFT:
		lightPos[0] -= LIGHT_V;
		break;
	case LRIGHT:
		lightPos[0] += LIGHT_V;
		break;
	case LFORWARD:
		lightPos[2] -= LIGHT_V;
		break;
	case LBACKWARD:
		lightPos[2] += LIGHT_V;
		break;
	case LUP:
		lightPos[1] += LIGHT_V;
		break;
	case LDOWN:
		lightPos[1] -= LIGHT_V;
		break;
	default: break;
	}
}

void Light::spot(LightDirec direc)
{
	if (light_onff > 0 || light_mode != SPOT_LIGHT)return;
	switch (direc) {
	case LLEFT:
		angle_lng -= LIGHT_A;
		if (angle_lng < -2 * PI) angle_lng = 0;
		break;
	case LRIGHT:
		angle_lng += LIGHT_A;
		if (angle_lng > 2 * PI) angle_lng = 0;
		break;
	case LUP:
		angle_lat += LIGHT_A;
		angle_lat = MIN(angle_lat, PI / 2);
		break;
	case LDOWN:
		angle_lat -= LIGHT_A;
		angle_lat = MAX(angle_lat, -PI / 2);
		break;
	default: break;
	}

	spotDirec[0] = CAMERA_R * cos(angle_lat) * sin(angle_lng);
	spotDirec[2] = CAMERA_R * cos(angle_lat) * cos(angle_lng);
	spotDirec[1] = CAMERA_R * sin(angle_lat);
}

void Light::update()
{
	glDisable(GL_LIGHT0);
}

GLenum Light::accquireLightSrc()
{
	for (int i = 0; i < 7; ++i) {
		if ((light_src[i] & 0x8000) == 0) {
			GLenum li = light_src[i];
			light_src[i] ^= 0x8000;
			return li;
		}
	}
	return 0x8000;
}

void Light::releaseLightSrc(GLenum light)
{
	if (light == 0x8000)return;
	int idx = light - GL_LIGHT1;
	if ((light_src[idx] & 0x8000) == 1) {
		light_src[idx] ^= 0x8000;
		glDisable(light);
	}
}