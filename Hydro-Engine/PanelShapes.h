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
	Primitive cube;
	Primitive sphere;
	Primitive cylinder;
	Primitive plane;
};
