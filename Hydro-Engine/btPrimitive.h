#pragma once

#include "MathGeoLib/include/MathGeoLib.h"
#include "Color.h"

enum class PrimitiveT
{
	PRIM_POINT,
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
};
