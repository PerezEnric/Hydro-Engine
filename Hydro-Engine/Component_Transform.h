#pragma once

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class Component_Transform : public Component
{
public:

	Component_Transform(GameObject* GO, COMPONENT_TYPE type);
	Component_Transform();

	float3 GetPosition();
	float3 GetRotation();
	float3 GetScale();

	void SetPosition(float3 pos);
	void SetRotation(float3 rot);
	void SetScale(float3 sca);


	void NewTransform();

	Component_Transform* GetThis();

	void ShowInfo();

	void LoadTransform(float3 pos, float3 scale, Quat rotation);
	float4x4 GetGlobalMatrix();

public:

	float4x4 my_current_matrix;
	float4x4 my_global_matrix;
	float3 l_position = float3( 0,0,0 );
	float3 l_scale = float3(1.0f, 1.0f, 1.0f);
	//rotation is a quat because they are cool.
	Quat l_rotation = Quat( 0,0,0,0 );
	float3 future_rotation = float3(0, 0, 0);
};



