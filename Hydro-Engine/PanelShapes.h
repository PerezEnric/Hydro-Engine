#pragma once

#include "Panel.h"
#include "Primitive.h"

class PanelShapes:public Panel 
{
public:
	PanelShapes();
	~PanelShapes();

	bool Update() override;

private:
	Primitive prim;
	par_shapes_mesh* p_mesh;
	bool is_created = false;
	int current_cube = 0;
	int current_sphere = 0;
	int current_cylinder = 0;
	int current_plane = 0;
	float3 cube_size = float3(1.0f, 1.0f, 1.0f);
	float sphere_rad = 1.0f;
	float cylinder_rad = 1.0f;
	float cylinder_height = 1.0f;
public:
	bool show_shapes = false;
};
