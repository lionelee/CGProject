#include "objmodel.h"
#include "math.h"
#include "macros.h"
#include "map"

char* model_name[] = { "", "./model/grimace/grimace.obj" };
const float MAX_NORMAL_ANGLE = 179.0f;
std::map<int, GLMmodel*> objs;

ObjModel::ObjModel(int type) 
{
	stat = DEAD;
	model_obj = objs[type];
}

ObjModel::~ObjModel()
{
	if (model_obj != NULL)
		delete model_obj;
}

void ObjModel::activate(float speed_x, float speed_y, float speed_z,
	float acc_x, float acc_y, float acc_z, float pos_x, float pos_y, float pos_z)
{
	stat = ACTIVE;
	v_x = speed_x; v_y = speed_y; v_z = speed_z;
	a_x = acc_x; a_y = acc_y; a_z = acc_z;
	p_x = pos_x; p_y = pos_y; p_z = pos_z;
	origin_y = pos_y;

	update_times = 0;
	flag = true;
}

void ObjModel::update()
{
	if (stat == DEAD) return;
	if (p_y <= 1.0) {
		if (fabsf(v_y) <= 0.5) {
			stat = DEAD;
			return;
		}
		v_y = -v_y;
	}
	if (p_x <= 0 || p_x >= SCENE_SIZE) v_x = -v_x;
	if (p_z <= 0 || p_z >= SCENE_SIZE) v_z = -v_z;
	if (p_y <= 0.1 && p_y >= origin_y) v_y = 0;
	++update_times;
	a_x = wind_a_x;
	a_z = wind_a_z;
	float a = -OBJ_AIR_RES * v_y;
	v_x += a_x;
	v_y += a_y + a;
	v_z += a_z;
	p_x += v_x;
	p_y += v_y;
	p_z += v_z;
}

void ObjModel::render()
{
	glPushMatrix();
	glTranslatef(p_x, p_y, p_z);
	glmUnitize(model_obj);
	glmScale(model_obj, 4.0f);
	glmDraw(model_obj, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}

void initObjModel(int type) {
	if (objs.count(type) > 0)return;
	GLMmodel* model_obj = glmReadOBJ(model_name[type]);
	if (!model_obj) exit(0); //fail
	glmFacetNormals(model_obj);
	glmVertexNormals(model_obj, MAX_NORMAL_ANGLE);
	objs[type] = model_obj;
}