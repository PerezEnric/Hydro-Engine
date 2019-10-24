#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Component_Transform : public Component
{
public:

	Component_Transform(GameObject* GO, COMPONENT_TYPE type);
	Component_Transform();

	float3 GetPosition();
	float3 GetRotation();
	float3 GetScale();

private:
	
	float3 GO_position = { 0, 0, 0 };
	float3 GO_rotation = { 0, 0, 0 };
	float3 GO_scale = { 1, 1, 1 };
};



