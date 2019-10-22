#pragma once

class GameObject;



enum COMPONENT_TYPE
{
	MESH,
	TEXTURE,
	TRANSFORM,


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

	GameObject* GO;
	bool active;
	COMPONENT_TYPE type = NONE;
};
