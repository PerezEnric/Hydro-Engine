#pragma once

#include "MathGeoLib/include/MathGeoLib.h"
#include "Color.h"
#include "PhysBody.h"

enum class PrimitiveT
{
	PRIM_POINT,
	PRIM_LINE,
	PRIM_CUBE,
	PRIM_SPHERE,
	PRIM_CYLINDER,
	PRIM_CAPSULE
};

class btPrimitive 
{
public:
	btPrimitive();
	virtual void Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3& u);
	void			Scale(float x, float y, float z);
	PrimitiveT	GetType() const;

public:

	Color color;
	float4x4 transform;
	bool axis, wire;
	PrimitiveT type;
	PhysBody* my_body;
};

class btCube : public btPrimitive
{
public:
	btCube();
	btCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;

public:

	float3 size;
};

class btSphere : public btPrimitive
{
public:
	btSphere();
	btSphere(float radius);
	void InnerRender() const;

public:
	float radius;
};

class btCylinder :public btPrimitive
{
public:
	btCylinder();
	btCylinder(float radius, float height);
	void InnerRender() const;

public:
	float radius, height;
};

class btLine : public btPrimitive
{
public:
	btLine();
	btLine(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};