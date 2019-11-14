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

public:
	Frustum		frustum;

};
