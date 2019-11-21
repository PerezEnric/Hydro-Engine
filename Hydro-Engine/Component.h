#pragma once

#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Geometry/OBB.h"
#include "Json/json.hpp"

class GameObject;



enum COMPONENT_TYPE
{
	MESH,
	TEXTURE,
	TRANSFORM,
	CAMERA,

	NONE
};

NLOHMANN_JSON_SERIALIZE_ENUM(COMPONENT_TYPE, {
	{MESH, "c_mesh"},
	{TEXTURE, "c_texture"},
	{TRANSFORM, "c_transform"},
	{CAMERA, "c_camera"},
	})

class Component
{
public:

	Component(GameObject* GO, COMPONENT_TYPE type, bool _empty) :
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

	virtual nlohmann::json SaveComponent() 
	{
		return nlohmann::json();
	}

	virtual void LoadComponent(nlohmann::json & to_load){}

	GameObject* GO;
	bool active;
	COMPONENT_TYPE type = NONE;
	std::string comp_type_str;
};
