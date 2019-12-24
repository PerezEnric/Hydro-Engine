#include "Application.h"
#include "Vehicle.h"
#include "btPrimitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"


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
	btCylinder wheel;

	wheel.color = Black; // Esto lo podemos cambiar.

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = car_info.wheels[0].radius;
		wheel.height = car_info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(*wheel.transform.v);

		wheel.Render();
	}

	btCube chassis(car_info.chassis_size.x, car_info.chassis_size.y, car_info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(*chassis.transform.v);
	chassis.color = Black;
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(car_info.chassis_position.x, car_info.chassis_position.y, car_info.chassis_position.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.SetTranslatePart(offset.getX(), offset.getY(), offset.getZ());

	chassis.Render();

	btCube cabin(car_info.cabin_size.x, car_info.cabin_size.y, car_info.cabin_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(*cabin.transform.v);
	cabin.color = Black;
	btQuaternion q_c = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_c(car_info.cabin_position.x, car_info.cabin_position.y, car_info.cabin_position.z);
	offset_c = offset_c.rotate(q_c.getAxis(), q_c.getAngle());

	cabin.transform.SetTranslatePart(offset_c.getX(), offset_c.getY(), offset_c.getZ());

	cabin.Render();

	btCube back(car_info.car_back_size.x, car_info.car_back_size.y, car_info.car_back_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(*back.transform.v);
	back.color = Black;
	btQuaternion q_b = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset_b(car_info.car_back_position.x, car_info.car_back_position.y, car_info.car_back_position.z);
	offset_b = offset_b.rotate(q_b.getAxis(), q_b.getAngle());

	back.transform.SetTranslatePart(offset_b.getX(), offset_b.getY(), offset_b.getZ());

	back.Render();
}

void Vehicle::ApplyEngineForce(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (car_info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

void Vehicle::Brake(float force)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (car_info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

void Vehicle::Turn(float degrees)
{
	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if (car_info.wheels[i].steering == true)
		{
			if (car_info.wheels[i].front == true)
			{
				vehicle->setSteeringValue(degrees, i);
			}
		}
	}
}

float Vehicle::GetKmh() const
{
	return 0.0f;
}

VehicleInfo::~VehicleInfo()
{
}
