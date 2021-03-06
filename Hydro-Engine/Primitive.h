
#pragma once
#include "glmath.h"
#include "Color.h"

#include "ParShapes/par_shapes.h"
#include "MathGeoLib/include/MathGeoLib.h"
#include "MathGeoLib/include/MathBuildConfig.h"
#include "MathGeoLib/include/MathGeoLibFwd.h"

class Component_Mesh;

enum PrimitiveTypes
{
	P_PLANE,
	P_CUBE,
	P_SPHERE,
	P_CYLINDER,
	P_NONE
};

NLOHMANN_JSON_SERIALIZE_ENUM(PrimitiveTypes, {
	{P_PLANE, "p_plane"},
	{P_CUBE, "p_cube"},
	{P_SPHERE, "p_sphere"},
	{P_CYLINDER, "p_cylinder"},
	{P_NONE, "p_none"},
	})

class Primitive
{
public:

	Primitive();

	virtual void	CreatePrimitive(PrimitiveTypes p_type, Component_Mesh* Ret, const float* axis = nullptr, 
					math::float3 t_vector = { 0.0f , 0.0f, 0.0f }, float radians = 0.0f, math::float3 s_vector = { 1.0f , 1.0f, 1.0f });

	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	uint m_vertices = 0;
	uint m_indices = 0;

protected:
	PrimitiveTypes type;
};

