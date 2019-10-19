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
public:
	bool show_shapes = false;
};
