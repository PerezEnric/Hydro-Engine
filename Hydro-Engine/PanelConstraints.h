#pragma once

#include "Panel.h"
#include "PhysBody.h"
#include "btPrimitive.h"

class btCube;

class PanelConstraints : public Panel
{
public:
	PanelConstraints();
	~PanelConstraints();


	bool Update() override;
	void CreateConstraint(float3 anchora, float3 anchorb);

public:
	//bool is_active = true;
	bool show_panelconstr = false;
	float3 anchorA = { 1.0f, 1.0f, 1.0f };
	float3 anchorB = { 1.0f, 1.0f, 1.0f };
};
