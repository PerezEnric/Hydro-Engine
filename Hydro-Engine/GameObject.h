#pragma once

#include "Globals.h"
#include "Component.h"
#include "Primitive.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"
#include "Component_Camera.h"
#include "ModuleSceneIntro.h"
#include "Json/json.hpp"

#include <string>
#include <vector>

class Component_Texture;
class Component_Mesh;
class Component_Transform;
class Component_Camera;

class GameObject
{
public:
	GameObject(const std::string& name);
	GameObject(const std::string& name, const std::string& Filename, int index);
	GameObject(const std::string& name, PrimitiveTypes type);
	GameObject(const std::string& name, const std::string& Filename, bool root);
	~GameObject();

	void Update();
	Component* CreateComponent(COMPONENT_TYPE type);
	void Cleanup();

	void CreateChildren(const std::string& name, const std::string& Filename, int index);

	void EliminateComponent(COMPONENT_TYPE type);

	void ShowInfo(COMPONENT_TYPE type);

	bool DoIhave(COMPONENT_TYPE type);

	void QuadTree(int n);

	void CreateEmptyChild(const std::string & name, const std::string& Filename);

	void SaveGameObject(nlohmann::json & to_save);

	AABB CreateAABB();
	OBB CreateOBB();

public:

	bool _static = true;


	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
	GameObject* parent = nullptr;

	int mesh_array = 0;
	int actual_mesh = 0;
	bool texture = false;
	std::string path;
	std::string texture_path;

	PrimitiveTypes p_type = P_NONE;

	Component_Texture* my_tex = nullptr;
	Component_Mesh* my_mesh;
	Component_Transform* transform;
	Component_Camera* cam;
	uint my_uuid = 0;
};