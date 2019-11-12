#pragma once

#include "Globals.h"
#include "Component.h"
#include "Primitive.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"

#include <string>
#include <vector>

class Component_Texture;
class Component_Mesh;
class Component_Transform;

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

	void QuadTree(int n);

	void CreateEmptyChild(const std::string & name, const std::string& Filename);

	AABB CreateBBox();

public:


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
	Component_Mesh my_mesh;
	Component_Transform transform;

	float3 GO_position = float3(0, 0, 0);
	float3 GO_rotation = float3(0, 0, 0);
	float3 GO_scale = float3(1.0f, 1.0f, 1.0f);
};