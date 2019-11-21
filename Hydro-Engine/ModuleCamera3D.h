#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Component_Camera.h"

class Component_Camera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	//void LookAt(const float3 &Spot);
	//void Move(const float3 &Movement);
	float* GetViewMatrix();
	void CentreGOView();
	void CastRay();

private:

	//void CalculateViewMatrix();

public:
	
	float3 X, Y, Z, Position, Reference;
	Component_Camera* main_cam = nullptr;
	Component_Camera* Test = nullptr;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};