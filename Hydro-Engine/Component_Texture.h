#pragma once

#include "Component.h"
#include "Globals.h"
#include <string>




class Component_Texture : public Component
{
public:

	Component_Texture(GameObject* GO, COMPONENT_TYPE type);
	Component_Texture();
	void Load_Texture();
	

public:

	uint id_texture = 0;
	uint widht = 0;
	uint height = 0;



};
