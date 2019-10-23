#pragma once

#include "Globals.h"
#include "Component.h"
#include "Primitive.h"

#include <string>
#include <vector>

class Component_Texture;

class GameObject
{
public:
	GameObject(const std::string& name);
	GameObject(const std::string& name, const std::string& Filename, int index);
	GameObject(const std::string& name, PrimitiveTypes type);
	~GameObject();

	void Update();
	Component* CreateComponent(COMPONENT_TYPE type);
	void Cleanup();

public:


	std::string name;
	std::vector<Component*> components;
	int mesh_array = 0;
	int actual_mesh = 0;
	bool texture = false;
	std::string path;
	std::string texture_path;

	PrimitiveTypes p_type = P_NONE;


	Component_Texture* my_tex = nullptr;



};