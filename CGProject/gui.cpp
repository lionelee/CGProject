#include "gui.h"
#include "stdio.h"
#include "macros.h"
#include "scene.h"
#include "tree.h"
#include "fireworkbox.h"

extern GUI gui;
extern Scene scene;
extern float wind_a_x;
extern float wind_a_z;


extern int main_window;

extern int light_onff, light_mode;
extern float cutoff, exponent;
extern float ambientLight[4], diffuseLight[4], specularLight[4], lightPos[4];
extern float const_atten, linear_atten, quadr_atten;

extern int flag_stop, flag_endless, flag_ch2obj;
int uobj = 0, layers = 1;
int fbposx = SCENE_SIZE / 2, fbposz = SCENE_SIZE / 2;
int removfbposx = SCENE_SIZE / 2, removfbposz = SCENE_SIZE / 2;
int tposx = 120, tposz = 120, level = 3;
int removtposx = 120, removtposz = 120;
float ratio = 0.7f, angle = 28.0f, height = 15.0f;

void GUI::init(int win_id)
{
	glui2 = GLUI_Master.create_glui_subwindow(win_id, GLUI_SUBWINDOW_RIGHT);
	glui1 = GLUI_Master.create_glui_subwindow(win_id, GLUI_SUBWINDOW_RIGHT);
	
	/*	controrl panel of global parameters */
	panel_coefficient = glui1->add_panel("Global Parameter");
	glui1->add_spinner_to_panel(panel_coefficient, "Wind_Acceleration_X",GLUI_SPINNER_FLOAT, &wind_a_x)->set_float_limits(-0.2f, 0.2f);
	glui1->add_spinner_to_panel(panel_coefficient, "Wind_Acceleration_Z",GLUI_SPINNER_FLOAT, &wind_a_z)->set_float_limits(-0.2f, 0.2f);

	/* controrl panel of light*/
	panel_light = glui1->add_panel("Light");
	GLUI_RadioGroup*  group = glui1->add_radiogroup_to_panel(panel_light, &light_onff);
	glui1->add_radiobutton_to_group(group, "light on");
	glui1->add_radiobutton_to_group(group, "light off");
	GLUI_Panel* panel_ligroup = glui1->add_panel_to_panel(panel_light, "Light Mode");
	ligroup = glui1->add_radiogroup_to_panel(panel_ligroup, &light_mode);
	glui1->add_radiobutton_to_group(ligroup, "point light");
	glui1->add_radiobutton_to_group(ligroup, "parallel light");
	glui1->add_radiobutton_to_group(ligroup, "spot light");
	
	GLUI_Panel* panel_ambient = glui1->add_panel_to_panel(panel_light, "Ambient");
	glui1->add_spinner_to_panel(panel_ambient, "red:", GLUI_SPINNER_FLOAT, &ambientLight[0])->set_float_limits(0.0f, 1.0f);
	glui1->add_spinner_to_panel(panel_ambient, "green:", GLUI_SPINNER_FLOAT, &ambientLight[1])->set_float_limits(0.0f, 1.0f);
	glui1->add_spinner_to_panel(panel_ambient, "blue:", GLUI_SPINNER_FLOAT, &ambientLight[2])->set_float_limits(0.0f, 1.0f);

	GLUI_Panel* panel_diffuse = glui1->add_panel_to_panel(panel_light, "Diffuse");
	glui1->add_spinner_to_panel(panel_diffuse, "red:", GLUI_SPINNER_FLOAT, &diffuseLight[0])->set_float_limits(0.0f, 1.0f);
	glui1->add_spinner_to_panel(panel_diffuse, "green:", GLUI_SPINNER_FLOAT, &diffuseLight[1])->set_float_limits(0.0f, 1.0f);
	glui1->add_spinner_to_panel(panel_diffuse, "blue:", GLUI_SPINNER_FLOAT, &diffuseLight[2])->set_float_limits(0.0f, 1.0f);

	GLUI_Panel* panel_specular = glui1->add_panel_to_panel(panel_light, "Specular");
	glui1->add_spinner_to_panel(panel_specular, "red:", GLUI_SPINNER_FLOAT, &specularLight[0])->set_float_limits(0.0f, 1.0f);
	glui1->add_spinner_to_panel(panel_specular, "green:", GLUI_SPINNER_FLOAT, &specularLight[1])->set_float_limits(0.0f, 1.0f);
	glui1->add_spinner_to_panel(panel_specular, "blue:", GLUI_SPINNER_FLOAT, &specularLight[2])->set_float_limits(0.0f, 1.0f);

	GLUI_Panel* panel_attenuation = glui1->add_panel_to_panel(panel_light, "Attenuation");
	glui1->add_spinner_to_panel(panel_attenuation, "constant:", GLUI_SPINNER_FLOAT, &const_atten)->set_float_limits(1.0f, 10.0f);
	glui1->add_spinner_to_panel(panel_attenuation, "linear:", GLUI_SPINNER_FLOAT, &linear_atten)->set_float_limits(0.0f, 1.0f);
	glui1->add_spinner_to_panel(panel_attenuation, "quadratic:", GLUI_SPINNER_FLOAT, &quadr_atten)->set_float_limits(0.0f, 1.0f);

	GLUI_Panel* panel_spotlight = glui1->add_panel_to_panel(panel_light, "SpotLight");
	glui1->add_spinner_to_panel(panel_spotlight, "cutoff:", GLUI_SPINNER_FLOAT, &cutoff)->set_float_limits(0.0f, 90.0f);
	glui1->add_spinner_to_panel(panel_spotlight, "exponent:", GLUI_SPINNER_FLOAT, &exponent)->set_float_limits(0.0f, 100.0f);

	GLUI_Panel* panel_position = glui1->add_panel_to_panel(panel_light, "Position");
	glui1->add_spinner_to_panel(panel_position, "posx:", GLUI_SPINNER_FLOAT, &lightPos[0]);
	glui1->add_spinner_to_panel(panel_position, "posy:", GLUI_SPINNER_FLOAT, &lightPos[1]);
	glui1->add_spinner_to_panel(panel_position, "posz:", GLUI_SPINNER_FLOAT, &lightPos[2]);


	/* controrl panel of firework*/
	panel_firework = glui2->add_panel("Firework");
	GLUI_Panel* panel_fbox = glui2->add_panel_to_panel(panel_firework, "Create Firework Box");
	GLUI_Spinner* spin_fbpx = glui2->add_spinner_to_panel(panel_fbox, "Posx:", GLUI_SPINNER_INT, &fbposx);
	int padding = (3*GRID_SIZE) / 2;
	spin_fbpx->set_int_limits(padding, SCENE_SIZE - padding);
	GLUI_Spinner* spin_fbpz = glui2->add_spinner_to_panel(panel_fbox, "Posz:", GLUI_SPINNER_INT, &fbposz);
	spin_fbpz->set_int_limits(padding, SCENE_SIZE - padding);
	glui2->add_spinner_to_panel(panel_fbox, "Firework Layer:", GLUI_SPINNER_INT, &layers)->set_int_limits(1, 4);
	GLUI_Panel* panel_fbgroup = glui2->add_panel_to_panel(panel_fbox, "Subparticle type");
	fbgroup = glui2->add_radiogroup_to_panel(panel_fbgroup);
	glui2->add_radiobutton_to_group(fbgroup, "firework");
	glui2->add_radiobutton_to_group(fbgroup, "grimace");
	glui2->add_button_to_panel(panel_fbox, "Create Firework Box",BTN_ADD_FBOX, controlFirework);

	GLUI_Panel* panel_rfbox = glui2->add_panel_to_panel(panel_firework, "Remove Firework Box");
	glui2->add_spinner_to_panel(panel_rfbox, "Posx:", GLUI_SPINNER_INT, &removfbposx)->set_int_limits(padding, SCENE_SIZE - padding);
	glui2->add_spinner_to_panel(panel_rfbox, "Posz:", GLUI_SPINNER_INT, &removfbposz)->set_int_limits(padding, SCENE_SIZE - padding);
	glui2->add_button_to_panel(panel_rfbox, "Remove Firework Box", BTN_DEL_FBOX, controlFirework);

	glui2->add_checkbox_to_panel(panel_firework, "using obj model", &flag_ch2obj);
	glui2->add_checkbox_to_panel(panel_firework, "Endless Mode", &flag_endless);
	glui2->add_checkbox_to_panel(panel_firework, "Stop All", &flag_stop);

	/* controrl panel of tree*/
	panel_tree = glui2->add_panel("Tree");
	GLUI_Panel* panel_ntree = glui2->add_panel_to_panel(panel_tree, "Create Tree");
	glui2->add_spinner_to_panel(panel_ntree, "Posx:", GLUI_SPINNER_INT, &tposx)->set_int_limits(GRID_SIZE, SCENE_SIZE - GRID_SIZE);
	glui2->add_spinner_to_panel(panel_ntree, "Posz:", GLUI_SPINNER_INT, &tposz)->set_int_limits(GRID_SIZE, SCENE_SIZE - GRID_SIZE);
	glui2->add_spinner_to_panel(panel_ntree, "Level:", GLUI_SPINNER_INT, &level)->set_int_limits(1, LEVEL_MAX);
	glui2->add_spinner_to_panel(panel_ntree, "Ratio:", GLUI_SPINNER_FLOAT, &ratio)->set_float_limits(0.1, 0.9);
	glui2->add_spinner_to_panel(panel_ntree, "Angle:", GLUI_SPINNER_FLOAT, &angle)->set_float_limits(15, 35);
	glui2->add_spinner_to_panel(panel_ntree, "Height:", GLUI_SPINNER_FLOAT, &height)->set_float_limits(10.0f, 25.0f);
	glui2->add_button_to_panel(panel_ntree, "Create Tree", BTN_ADD_TREE, controlTree);

	GLUI_Panel* panel_rtree = glui2->add_panel_to_panel(panel_tree, "Remove Tree");
	glui2->add_spinner_to_panel(panel_rtree, "Posx:", GLUI_SPINNER_INT, &removtposx)->set_int_limits(GRID_SIZE, SCENE_SIZE - GRID_SIZE);
	glui2->add_spinner_to_panel(panel_rtree, "Posz:", GLUI_SPINNER_INT, &removtposz)->set_int_limits(GRID_SIZE, SCENE_SIZE - GRID_SIZE);
	glui2->add_button_to_panel(panel_rtree, "Remove Tree", BTN_DEL_TREE, controlTree);

}

void controlLight(int control)
{
	
}

void controlFirework(int control)
{
	int x = 0, z = 0;
	switch (control)
	{
	case BTN_ADD_FBOX:
		x = fbposx / GRID_SIZE, z = fbposz / GRID_SIZE;
		if (scene.occupy(x, z, 1)) {
			addFireworkbox(x*GRID_SIZE + GRID_SIZE / 2, z*GRID_SIZE + GRID_SIZE / 2, layers, gui.fbgroup->get_int_val());
		}
		break;
	case BTN_DEL_FBOX:
		x = removfbposx / GRID_SIZE, z = removfbposz / GRID_SIZE;
		if (scene.release(x, z, 1)) {
			removeFireworkbox(x*GRID_SIZE + GRID_SIZE / 2, z*GRID_SIZE + GRID_SIZE / 2);
		}
		break;
	default:
		break;
	}
}

void controlTree(int control)
{
	int x = 0,  z = 0;
	switch (control)
	{
	case BTN_ADD_TREE:
		x = tposx / GRID_SIZE, z = tposz / GRID_SIZE;
		if (scene.occupy(x, z, 2)){
			addTree(x*GRID_SIZE, z*GRID_SIZE, level, ratio, angle, height);
		}
		break;
	case BTN_DEL_TREE:
		x = removtposx / GRID_SIZE, z = removtposz / GRID_SIZE;
		if (scene.release(x, z, 2)) {
			removeTree(x*GRID_SIZE, z*GRID_SIZE);
		}
	default:
		break;
	}
}
