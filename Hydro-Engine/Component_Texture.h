#pragma once

#include "Component.h"
#include "Globals.h"
#include <string>


class ResourceTexture;

class Component_Texture : public Component
{
public:

	Component_Texture(GameObject* GO, COMPONENT_TYPE type, bool _empty);
	Component_Texture();
	void Load_Texture();

	Component_Texture* GetThis();

	void ShowInfo();
	
	void CleanUp();

	void CleanResUp();

	uint PointerToText();

	//Save and Load
	nlohmann::json SaveComponent();
	void LoadComponent(nlohmann::json & to_load);

public:

	uint id_texture = 0;
	uint widht = 0;
	uint height = 0;
	std::string own_format;

	uint UUID_resource;
	ResourceTexture* my_reference = nullptr;
};
