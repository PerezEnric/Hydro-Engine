#include "PhysBody.h"
#include "Bullet/include/btBulletDynamicsCommon.h"


PhysBody::PhysBody(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}

PhysBody::~PhysBody()
{
	delete body;
}

void PhysBody::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

void PhysBody::GetTransform(float* matrix) const
{
	if (body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

void PhysBody::SetTransform(const float* matrix) const
{
	if (body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

void PhysBody::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

float3 PhysBody::GetPos(float& x, float& y, float& z)
{
	float3 pos;
	btTransform t = body->getWorldTransform();
	btVector3 origin = t.getOrigin();
	x = (float)origin.getX();
	y = (float)origin.getY();
	z = (float)origin.getZ();
	pos.x = x;
	pos.y = y;
	pos.z = z;

	return pos;
}