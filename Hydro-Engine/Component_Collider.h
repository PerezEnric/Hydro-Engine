#pragma once
#include "Component.h"

struct PhysBody;

class Component_Collider : public Component
{

	Component_Collider(GameObject* GO, COMPONENT_TYPE type, bool _empty);
	Component_Collider();

	void ShowInfo();

	void CleanUp();

	bool Update();

	void CleanResUp();

	//Save and Load


	nlohmann::json SaveComponent();


	void LoadComponent(nlohmann::json & to_load);

public:
	PhysBody* my_phis_body;

};