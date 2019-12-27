#pragma once

#include "Module.h"
#include "Globals.h"

#include "MathGeoLib/include/MathGeoLib.h"

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD

struct Vehicle;

class ModuleVehicle : public Module
{
public:
	ModuleVehicle(Application* app, bool start_enabled = true);
	virtual ~ModuleVehicle();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

public:

	Vehicle* vehicle;
	float turn;
	float acceleration;
	float brake;
	float3 position = { 0.0f,0.0f,0.0f };
	float3 initial_position = { 0.0f,20.0f,0.0f };
	float3 current_position;

	bool over_cam = false;

};
