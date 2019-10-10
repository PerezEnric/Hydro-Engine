
#pragma once
#include "glmath.h"
#include "Color.h"

#include "ParShapes/par_shapes.h"

enum PrimitiveTypes
{
	P_PLANE,
	P_CUBE,
	P_SPHERE,
	P_CYLINDER
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() ;
	virtual void InnerRender() const;
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	uint m_vertices = 0;
	uint m_indices = 0;
	uint shape_indices = 0;
	par_shapes_mesh* mesh = nullptr;

protected:
	PrimitiveTypes type;
};

class Cube :public Primitive 
{
public:
	Cube();
	~Cube();

	void InnerRender();
};

