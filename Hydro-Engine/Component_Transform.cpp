#include "Application.h"
#include "GameObject.h"
#include "Component_Transform.h"
#include "ModuleSceneIntro.h"
#include "ImGui/imgui.h"

Component_Transform::Component_Transform(GameObject* GO, COMPONENT_TYPE type): Component(GO, type)
{
	// first of all we calculate our matrix
	my_current_matrix = float4x4::FromTRS(l_position, l_rotation, l_scale);

	if (GO->parent != nullptr)
	{
		my_global_matrix = GO->parent->transform.my_global_matrix * my_current_matrix; // here we need to see what is our matrix in the world
	}
	else
		my_global_matrix = my_current_matrix;

	//test
	
	my_global_matrix = my_current_matrix;
}

Component_Transform::Component_Transform()
{
}

float3 Component_Transform::GetPosition()
{

}

float3 Component_Transform::GetRotation()
{
	
}

float3 Component_Transform::GetScale()
{

}

void Component_Transform::SetPosition(float3 position)
{
}

void Component_Transform::NewTransform()
{
	my_current_matrix = float4x4::FromTRS(l_position, l_rotation, l_scale);

	if (GO->parent != nullptr)
	{
		if (GO->DoIhave(TRANSFORM))
			my_global_matrix = GO->parent->transform.my_global_matrix * my_current_matrix;
		else
			my_global_matrix = my_current_matrix;
	}
	
	
	if (GO->childrens.size() > 0) // all the GO childrens need to know that we are transforming :D
	{
		for (uint i = 0; i < GO->childrens.size(); i++)
		{
			if (GO->childrens[i]->DoIhave(TRANSFORM))
				GO->childrens[i]->transform.NewTransform(); // methods?
		}
	}

}

Component_Transform * Component_Transform::GetThis()
{
	return this;
}

void Component_Transform::ShowInfo()
{
	ImGui::Text("Position: %.2f %.2f %.2f", l_position.x,
		l_position.y,
		l_position.z);

	ImGui::Text("Rotation: %.2f %.2f %.2f", l_rotation.x,
		l_rotation.y,
		l_rotation.z);

	ImGui::Text("Scale: %.2f %.2f %.2f",
		l_scale.x,
		l_scale.y,
		l_scale.z);
}
