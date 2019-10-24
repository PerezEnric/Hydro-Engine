#include "Component_Transform.h"
#include "GameObject.h"

Component_Transform::Component_Transform(GameObject* GO, COMPONENT_TYPE type): Component(GO, type)
{
}

Component_Transform::Component_Transform()
{
}

float3 Component_Transform::GetPosition()
{
	return float3(GO_position.x, GO_position.y, GO_position.z);
}

float3 Component_Transform::GetRotation()
{
	return float3(GO_rotation.x, GO_rotation.y, GO_rotation.z);
}

float3 Component_Transform::GetScale()
{
	return float3(GO_scale.x, GO_scale.y, GO_scale.z);
}

Component_Transform * Component_Transform::GetThis()
{
	return this;
}

void Component_Transform::ShowInfo()
{
}
