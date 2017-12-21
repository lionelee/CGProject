#include "gui.h"
#include "macros.h"

extern float wind_a_x;
extern float wind_a_z;
extern int flag_endless;

void GUI::init(int win_id)
{
	glui = GLUI_Master.create_glui("Control Panel", 0, WIN_LENGTH + WIN_POSX + 15, WIN_POSY);
	glui->set_main_gfx_window(win_id);

	/*	controrl panel of global parameters */
	panel_coefficient = glui->add_panel("Global Parameter");
	GLUI_Spinner* spn_wx = glui->add_spinner_to_panel(panel_coefficient, "Wind_Acceleration_X", GLUI_SPINNER_FLOAT,
		&wind_a_x, SPN_WIND_X);
	spn_wx->set_float_limits(0.0f, 1.0f);
	spn_wx->set_speed(10.0f);
	GLUI_Spinner* spn_wz = glui->add_spinner_to_panel(panel_coefficient, "Wind_Acceleration_Z", GLUI_SPINNER_FLOAT,
		&wind_a_z, SPN_WIND_Z);
	spn_wz->set_float_limits(0.0f, 1.0f);
	spn_wz->set_speed(10.0f);

	/* controrl panel of light*/
	panel_light = glui->add_panel("Light");
	glui->add_button_to_panel(panel_light, "Add Light");
	glui->add_button_to_panel(panel_light, "Remove Light");
	glui->add_button_to_panel(panel_light, "Adjust Light");

	/* controrl panel of firework*/
	panel_firework = glui->add_panel("Firework");
	glui->add_checkbox_to_panel(panel_firework, "Endless Mode", &flag_endless);

	/* controrl panel of tree*/
	panel_tree = glui->add_panel("Tree");
}

void GUI::paramter_cb(int control)
{
	switch (control){
	default: break;
	}
}