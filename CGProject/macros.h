#ifndef MACROS_H
#define MACROS_H

#define	MAX(a, b)			a > b ?	a : b
#define	MIN(a,	 b)		a < b ? a : b

//constants about glut window
#define	WIN_POSX		200
#define	WIN_POSY		200
#define	WIN_LENGTH	800
#define	WIN_WIDTH	600

//rotation radius camera
#define	CAMERA_R		1.0f
//minium rotation angle of camera
#define	CAMERA_A		PI/36
//moving speed of camera
#define	CAMERA_V		1.0f

//scene size
#define	SCENE_SIZE		200
//scene height
#define	SCENE_HT		100
//minium texture size
#define	GRID_SIZE	8
//texture numeber
#define	TX_NUM			8

//maxmium value of random number
#define	RAND_MAX		0X7fff

//circumference ratio
#define	PI						3.1415926f
//coefficient of gravity
#define	GRAVITY			0.98f

//height(y) of particle when active
#define	ACTIVE_Y			200.0f
//maxium num of particles
#define	PAR_MAX		4000

//fireworkbox size
#define	FBOX_SIZE		10
//number of firework particles within a box
#define	FP_PER_BOX	FBOX_SIZE * FBOX_SIZE  / 4
//coefficient of air resistance of firework particles
#define	FP_AIR_RES		0.6
//coefficient of air resistance of sub particles
#define	PA_AIR_RES		0.1
//speed of firework particle
#define	FP_V					10.0f
//speed of sub particle within firework particle
#define	PA_FP_V			1.0f

#endif MACROS_H
