#pragma once

#include "Module.h"
#include "Globals.h"
#include "btPrimitive.h"
#include <list>
#include "Bullet/include/btBulletDynamicsCommon.h"

#define GRAVITY btVector3(0.0f, -10.0f, 0.0f)

class DebugDrawer;

struct PhysBody;
struct Vehicle;
struct VehicleInfo;

class ModulePhysics : public Module
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	PhysBody* AddBody(const btSphere& sphere, float mass = 1.0f);
	PhysBody* AddBody(const btCube& cube, float mass = 1.0f);
	PhysBody* AddBody(const btCylinder& cylinder, float mass = 1.0f);
	//Vehicle* AddVehicle(const VehicleInfo& info);

	void AddConstraintP2P(PhysBody& bodyA, PhysBody& bodyB, const float3& anchorA, const float3& anchorB);
	void AddConstraintHinge(PhysBody& bodyA, PhysBody& bodyB, const float3& anchorA, const float3& anchorB, const float3& axisS, const float3& axisB, bool disable_collision = false, bool motor = false);

private:
	btDefaultCollisionConfiguration* collision_conf;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* world;
	btDefaultVehicleRaycaster* vehicle_raycaster;
	DebugDrawer* debug_draw;

	std::list<btCollisionShape*> shapes;
	std::list<PhysBody*> bodies;
	std::list<btDefaultMotionState*> motions;
	std::list<btTypedConstraint*> constraints;

	bool debug = true;
};



class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0, 0, 0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	btLine line;
	btPrimitive point;
};
