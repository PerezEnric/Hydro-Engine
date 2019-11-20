#pragma once

#include "Component.h"
#include "Globals.h"
#include <string>




class Component_Texture : public Component
{
public:

	Component_Texture(GameObject* GO, COMPONENT_TYPE type, bool _empty);
	Component_Texture();
	void Load_Texture();

	Component_Texture* GetThis();

	void ShowInfo();
	
	void CleanUp();

	nlohmann::json SaveComponent();

public:

	uint id_texture = 0;
	uint widht = 0;
	uint height = 0;
	std::string own_format;
};
