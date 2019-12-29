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

	void CentreGOView();
	void CastRay();
	void DrawRay();

public:
	
	float3 X, Y, Z, Position, Reference;
	Component_Camera* main_cam = nullptr;
	Component_Camera* Test = nullptr;
	LineSegment picking;

	bool draw_ray = false;

};