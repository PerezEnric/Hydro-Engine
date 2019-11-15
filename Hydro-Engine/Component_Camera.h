#pragma once

#include "Component.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;

class Component_Camera : public Component
{
public:
	Component_Camera(GameObject* gameObject, COMPONENT_TYPE type);
	~Component_Camera();

	bool Update();
	void FrustrumLook(const float3& position);
	void DrawFrustrum();
	void ShowInfo();

public:
	Frustum		frustum;
	bool show_frustum = false;
};
