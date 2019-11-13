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
		my_global_matrix = GO->parent->transform->my_global_matrix * my_current_matrix; // here we need to see what is our matrix in the world
	}
	else
		my_global_matrix = my_current_matrix;

	//test
	GO->transform = this;
	
	//my_global_matrix = my_current_matrix;
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
	l_position = position;
	//we calculate The new global matrix after we transform (we will call our childrens matrix)
	NewTransform();
}

void Component_Transform::SetRotation(float3 rot)
{
	// we will use euler to get the quaternion of rotation.
	l_rotation = Quat::FromEulerXYZ(rot.x * DEGTORAD, rot.y * DEGTORAD, rot.z * DEGTORAD);
	//we calculate The new global matrix after we transform (we will call our childrens matrix)
	NewTransform();
}

void Component_Transform::SetScale(float3 sca)
{
	l_scale = sca;
	//we calculate The new global matrix after we transform (we will call our childrens matrix)
	NewTransform();
}

void Component_Transform::NewTransform()
{
	my_current_matrix = float4x4::FromTRS(l_position, l_rotation, l_scale);

	if (GO->parent != nullptr) // Same as last time :D
	{
		if (GO->DoIhave(TRANSFORM))
			my_global_matrix = GO->parent->transform->my_global_matrix * my_current_matrix;
		else
			my_global_matrix = my_current_matrix;
	}
	
	
	if (GO->childrens.size() > 0) // all the GO childrens need to know that we are transforming :D
	{
		for (uint i = 0; i < GO->childrens.size(); i++)
		{
			if (GO->childrens[i]->DoIhave(TRANSFORM))
				GO->childrens[i]->transform->NewTransform(); // methods?
		}
	}

}

Component_Transform * Component_Transform::GetThis()
{
	return this;
}

void Component_Transform::ShowInfo()
{
	//gtodo: make that we can swap from 
	if (ImGui::DragFloat3("Position", &l_position[3], 0.1f, -10.0f, 10.0f))
		SetPosition(l_position);

	ImGui::Text("Position: %f %f %f", l_position.x,
		l_position.y,
		l_position.z);

	ImGui::Text("Rotation: %f %f %f", l_rotation.x,
		l_rotation.y,
		l_rotation.z);

	ImGui::Text("Scale: %f %f %f",
		l_scale.x,
		l_scale.y,
		l_scale.z);
}

void Component_Transform::LoadTransform(float3 pos, float3 scale, Quat rotation)
{
	this->l_position = pos;
	this->l_scale = scale;
	this->l_rotation = rotation;
}
