#pragma once

#include "Globals.h"
#include "Component.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/material.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include <string>
#include <vector>

class GameObject
{
public:
	GameObject(const std::string& name);
	GameObject(const std::string& name, const std::string& Filename);
	~GameObject();

	void Update();
	Component* CreateComponent(COMPONENT_TYPE type);




	

public:


	std::string name;
	std::vector<Component*> components;
	int mesh_array = 0;
	int actual_mesh = 0;
	bool texture = false;
	std::string path;



};