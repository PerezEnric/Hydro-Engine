#pragma once

#include <list>
#include "MathGeoLib/include/MathGeoLib.h"

class btRigidBody;
class Module;

struct PhysBody
{
	friend class ModulePhysics;
public:
	PhysBody(btRigidBody* body);
	~PhysBody();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	float3 GetPos(float& x, float& y, float& z);

	std::list<Module*> collision_listeners;
private:
	btRigidBody* body = nullptr;
};
