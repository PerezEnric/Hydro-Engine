#include "Component_Collider.h"

Component_Collider::Component_Collider(GameObject * GO, COMPONENT_TYPE type, bool _empty)
{
}

Component_Collider::Component_Collider()
{
}

void Component_Collider::ShowInfo()
{
}

void Component_Collider::CleanUp()
{
}

bool Component_Collider::Update()
{
	return false;
}

void Component_Collider::CleanResUp()
{
}

nlohmann::json Component_Collider::SaveComponent()
{
	return nlohmann::json();
}

void Component_Collider::LoadComponent(nlohmann::json & to_load)
{
}
