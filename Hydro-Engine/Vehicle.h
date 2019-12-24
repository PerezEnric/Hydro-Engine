#pragma once

#include "PhysBody.h"
#include "MathGeoLib/include/MathGeoLib.h"

class btRaycastVehicle;
struct PhysBody3D;

struct car_wheel
{
	float3 connection; 
	float3 direction;
	float3 axis;

	float suspensionRestLength;
	float radius;
	float width;

	bool front; 
	bool drive; 
	bool brake; 
	bool steering; 
};

struct VehicleInfo
{
	~VehicleInfo();

	float3 chassis_size; 
	float3 chassis_position; // position respects the center of the car.

	float3 cabin_size;
	float3 cabin_position; // position respects the center of the car.

	float3 car_back_size; // The back part of the car
	float3 car_back_position; // position respects the center of the car.

	// Usefull phisics variables.
	float mass;
	float suspensionStiffness; 
	float suspensionCompression; 
	float suspensionDamping;
	float maxSuspensionTravelCm; 
	float frictionSlip;
	float maxSuspensionForce; 

	// Car Wheels.
	car_wheel* wheels;
	int num_wheels;
	//do defaults.
};


struct Vehicle : public PhysBody
{
public:
	Vehicle(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~Vehicle();

	void Render();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;

public:
	btRigidBody* car_body;
	VehicleInfo car_info;
	btRaycastVehicle* vehicle;
};
