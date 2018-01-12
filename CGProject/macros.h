#ifndef MACROS_H
#define MACROS_H

#define	MAX(a, b)			a > b ?	a : b
#define	MIN(a,	 b)		a < b ? a : b

//constants about glut window
#define	WIN_POSX		5
#define	WIN_POSY		10
#define	WIN_LENGTH	1400
#define	WIN_WIDTH	800

//maxmium value of random number
#define	RAN_MAX		0X7fff
//circumference ratio
#define	PI						3.1415926f
//coefficient of gravity
#define	GRAVITY			0.98f

//rotation radius of camera
#define	CAMERA_R		1.0f
//minium rotation angle of camera
#define	CAMERA_A		PI / 360
//moving speed of camera
#define	CAMERA_V		1.0f

//rotation radius of light
#define	LIGHT_R			1.0f
//minium rotation angle of light
#define	LIGHT_A			PI / 36
//moving speed of light
#define	LIGHT_V			1.0f
//light mode
#define	POINT_LIGHT	0
#define	PARAL_LIGHT	1
#define	SPOT_LIGHT	2

//scene size
#define	SCENE_SIZE		200
//scene height
#define	SCENE_HT		100
//minium texture size
#define	GRID_SIZE		8
//texture numeber
#define	TX_NUM			16

//height(y) of particle when active
#define	ACTIVE_Y			200.0f
//maxium num of particles
#define	PAR_MAX		4000


//fireworkbox size
#define	FBOX_SIZE		10
//number of firework particles within a box
#define	FP_PER_BOX	FBOX_SIZE * FBOX_SIZE  / 4
//coefficient of air resistance of firework particles
#define	FP_AIR_RES		0.05
//coefficient of air resistance of sub particles
#define	PA_AIR_RES		0.04
//speed of firework particle
#define	FP_V					2.6f
//speed of sub particle within firework particle
#define	PA_FP_V			0.2f
//time interval between emission
#define	FP_EMT_INT	60
//maxium number of sub particles' layer
#define	PA_LAY_MAX	4
//time interval of sub particle to be dead
#define	PA_DEA_INT	24
//coefficient of air resistance of objmodel
#define	OBJ_AIR_RES	0.01


//slices of trunk cyliner
#define	TRK_SLICES		20
//stacks of trunk cylinder
#define	TRK_STACKS	20
//maxium number of recursive level
#define	LEVEL_MAX		6
//time interval of tree growing
#define	GROW_INT		60
//time interval of leaves fading
#define	FADE_INT		5
//time interval of leaves leaving tree
#define	LEAVE_INT		5
//time interval to  regrow
#define	REGW_INT		1
//coefficient of air resistance of leave
#define	LEA_AIR_RES	0.01

#endif MACROS_H
