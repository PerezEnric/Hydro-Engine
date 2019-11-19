#include "Application.h"
#include "GameObject.h"
#include "Component_Transform.h"
#include "ModuleSceneIntro.h"
#include "ImGui/imgui.h"

Component_Transform::Component_Transform(GameObject* GO, COMPONENT_TYPE type): Component(GO, type)
{
	comp_type_str = "transform";
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
	bbox_changed = true;

}

void Component_Transform::SetRotation(float3 rot)
{
	l_rotation = Quat::FromEulerXYZ(rot.x * DEGTORAD, rot.y * DEGTORAD, rot.z * DEGTORAD);


	//we calculate The new global matrix after we transform (we will call our childrens matrix)
	NewTransform();
	bbox_changed = true;

}

void Component_Transform::SetScale(float3 sca)
{

	l_scale = sca;
	//we calculate The new global matrix after we transform (we will call our childrens matrix)
	NewTransform();
	bbox_changed = true;
}

void Component_Transform::NewTransform()
{
	my_current_matrix = float4x4::FromTRS(l_position, l_rotation, l_scale);
	future_rotation = l_rotation.ToEulerXYZ(); // we set the quaternion into a float3 so we can use it in SetRotation()
	future_rotation *= RADTODEG; // the previous function returns the rotation in radians so we put in degrees

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
	
	if (ImGui::DragFloat3("Position", &l_position[3], 0.1f))
		SetPosition(l_position);

	if (ImGui::DragFloat3("Rotation", &future_rotation[3], 0.1f))
		SetRotation(future_rotation);

	if (ImGui::DragFloat3("Scale", &l_scale[3], 0.1f))
		SetRotation(l_scale);
}

void Component_Transform::LoadTransform(float3 pos, float3 scale, Quat rotation)
{
	this->l_position = pos;
	this->l_scale = scale;
	this->l_rotation = rotation;
}

float4x4 Component_Transform::GetGlobalMatrix()
{
	my_current_matrix = float4x4::FromTRS(l_position, l_rotation, l_scale);
	future_rotation = l_rotation.ToEulerXYZ(); // we set the quaternion into a float3 so we can use it in SetRotation()
	future_rotation *= RADTODEG; // the previous function returns the rotation in radians so we put in degrees

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


	return my_global_matrix;
}

nlohmann::json Component_Transform::SaveComponent()
{
	nlohmann::json ret;

	std::vector<float> float_helper;
	float_helper.push_back(l_position.x);
	float_helper.push_back(l_position.y);
	float_helper.push_back(l_position.z);

	ret["l_position"] = float_helper;

	float_helper.clear();

	float_helper.push_back(l_scale.x);
	float_helper.push_back(l_scale.y);
	float_helper.push_back(l_scale.z);

	ret["l_scale"] = float_helper;
	float_helper.clear();

	float_helper.push_back(l_rotation.x);
	float_helper.push_back(l_rotation.y);
	float_helper.push_back(l_rotation.z);
	float_helper.push_back(l_rotation.w);

	ret["l_rotation"] = float_helper;

	float_helper.clear();

	float_helper.push_back(future_rotation.x);
	float_helper.push_back(future_rotation.y);
	float_helper.push_back(future_rotation.z);
	

	ret["future_rotation"] = float_helper;

	float_helper.clear();
	return ret;
}
