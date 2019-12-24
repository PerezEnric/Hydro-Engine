#include "Vehicle.h"
#include "Bullet\include\btBulletDynamicsCommon.h"
#include "btPrimitive.h"

Vehicle::Vehicle(btRigidBody * body, btRaycastVehicle * vehicle, const VehicleInfo & info) : PhysBody(body), vehicle(vehicle), car_info(info)
{
	car_body = body;
}

Vehicle::~Vehicle()
{
	delete vehicle;
}

void Vehicle::Render()
{
	vehicle->getNumWheels();
}

void Vehicle::ApplyEngineForce(float force)
{
}

void Vehicle::Brake(float force)
{
}

void Vehicle::Turn(float degrees)
{
}

float Vehicle::GetKmh() const
{
	return 0.0f;
}

VehicleInfo::~VehicleInfo()
{
}
