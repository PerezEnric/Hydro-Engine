#pragma once

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"

class GameObject;



enum COMPONENT_TYPE
{
	MESH,
	TEXTURE,
	TRANSFORM,
	CAMERA,

	NONE
};

class Component
{
public:

	Component(GameObject* GO, COMPONENT_TYPE type) :
		type(type),
		GO(GO)
	{};

	Component()
	{
	}

	~Component() {

	};


	virtual bool Enable() {
		return active = false;
	}

	virtual bool Disable() {
		return active = true;
	}

	virtual bool Update() {
		return true;
	}

	virtual void ShowInfo() {

	}

	virtual AABB CreateAABB()
	{
		return AABB();
	}

	virtual OBB CreateOBB()
	{
		return OBB();
	}

	virtual void CleanUp() {}

	GameObject* GO;
	bool active;
	COMPONENT_TYPE type = NONE;
};
