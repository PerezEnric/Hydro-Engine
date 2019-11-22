#pragma once

#include "Resource.h"
#include "Globals.h"

class Component_Mesh;

class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uuid, RESOURCE_TYPE type);
	~ResourceMesh();

public:
	Component_Mesh* my_mesh;


};
