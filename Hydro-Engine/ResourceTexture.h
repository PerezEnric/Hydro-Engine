#pragma once
#include "Resource.h"
#include "Globals.h"

class Component_Texture;

class ResourceTexture : public Resource
{
public:
	ResourceTexture(uint uuid, RESOURCE_TYPE type);
	~ResourceTexture();
	bool LoadToMemory();
	void NotReference();
	bool UnLoadFromMemory();
	void CleanUp();
public:

	Component_Texture* my_tex = nullptr;
};

