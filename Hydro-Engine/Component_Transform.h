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

	void SetPosition(float3 position);

	Component_Transform* GetThis();

	void ShowInfo();

private:

	//float3 GO_position = float3(0, 0, 0);
	//float3 GO_rotation = float3(0, 0, 0);
	//float3 GO_scale = float3(1.0f, 1.0f, 1.0f);
};



