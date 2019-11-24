#pragma once

#include "Component.h"

#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLib.h"

class GameObject;
class Component_Transform;

enum Intersection_Type
{
	OUTSIDE,
	INSIDE
};

class Component_Camera : public Component
{
public:
	Component_Camera(GameObject* gameObject, COMPONENT_TYPE type, bool _empty);
	Component_Camera();
	~Component_Camera();

	bool Update();
	void FrustrumLook(const float3& position);
	void DrawFrustrum();
	void ShowInfo();
	void ChangeNearPlaneDist(float distance);
	void ChangeFarPlaneDist(float distance);
	void ChangeFOV(float angle);
	void SetFrustumPosition(float3 position);
	void SetFrustumTransform();
	void SetFrustumRotation(float3 rot);
	int ContainsAABBox(const AABB& refbox) const;
	float* GetViewMatrix() const;
	float4x4 GetProjectionMatrix() const;


	bool DoCulling(GameObject* go);

	//Save and Load
	nlohmann::json SaveComponent();
	void LoadComponent(nlohmann::json & to_load);


public:
	Frustum		frustum;
	bool show_frustum = false;
	float angle_fov;

private: 
	float4x4 my_current_matrix;
	float4x4 my_global_matrix;


	float3 l_scale = float3(1.0f, 1.0f, 1.0f);
	//rotation is a quat because they are cool.
	Quat l_rotation = Quat(0, 0, 0, 0);
	float3 future_rotation = float3(0, 0, 0);
	
};
