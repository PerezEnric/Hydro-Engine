#include "Globals.h"
#include <gl/GLU.h>
#include <gl/GL.h>
#include "glut/glut.h"
#include "btPrimitive.h"


#pragma comment (lib, "glut/glut32.lib")


btPrimitive::btPrimitive() : transform(transform.identity), color(White), wire(false), axis(false), type(PrimitiveT::PRIM_POINT)
{}

PrimitiveT btPrimitive::GetType() const
{
	return type;
}

float3 btPrimitive::GetPos()
{
	return float3();
}

void btPrimitive::Render() const
{
	glPushMatrix();
	glMultMatrixf(*transform.v); //matrix 4x4 pointer

	if (axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

void btPrimitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

void btPrimitive::SetPos(float x, float y, float z)
{
	this_pos.x = x;
	this_pos.y = y;
	this_pos.z = z;

	transform.At(3, 0) = this_pos.x;
	transform.At(3, 1) = this_pos.y;
	transform.At(3, 2) = this_pos.z;
}

void btPrimitive::SetRotation(float angle, const float3 &u)
{
	transform.SetRotatePart(u, angle);
}

void btPrimitive::Scale(float x, float y, float z)
{
	transform.Scale(x, y, z);
}

// CUBE

btCube::btCube() : btPrimitive(), size(1.0f, 1.0f, 1.0f)
{
	type = PrimitiveT::PRIM_CUBE;
}

btCube::btCube(float sizeX, float sizeY, float sizeZ) : btPrimitive(), size(sizeX, sizeY, sizeZ)
{
	type = PrimitiveT::PRIM_CUBE;
}

void btCube::InnerRender() const
{
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f(sx, -sy, sz);
	glVertex3f(sx, sy, sz);
	glVertex3f(-sx, sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, sy, -sz);
	glVertex3f(sx, sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy, sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx, sy, -sz);
	glVertex3f(sx, sy, sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy, sz);
	glVertex3f(-sx, sy, sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy, sz);
	glVertex3f(sx, sy, sz);
	glVertex3f(sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx, -sy, sz);
	glVertex3f(-sx, -sy, sz);

	glEnd();
}

// SPHERE
btSphere::btSphere() : btPrimitive(), radius(1.0f)
{
	type = PrimitiveT::PRIM_SPHERE;
}

btSphere::btSphere(float radius) : btPrimitive(), radius(radius)
{
	type = PrimitiveT::PRIM_SPHERE;
}

void btSphere::InnerRender() const
{
	glutSolidSphere(radius, 25, 25);
}

// Cylinder

btCylinder::btCylinder() : btPrimitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveT::PRIM_CYLINDER;
}

btCylinder::btCylinder(float radius, float height) : btPrimitive(), radius(radius), height(height)
{
	type = PrimitiveT::PRIM_CYLINDER;
}

void btCylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);

	for (int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * PI / 180; // degrees to radians
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * PI / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 480; i += (360 / n))
	{
		float a = i * PI / 180; // degrees to radians

		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
		glVertex3f(-height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();
}

btLine::btLine() : btPrimitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveT::PRIM_LINE;
}

btLine::btLine(float x, float y, float z) : btPrimitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveT::PRIM_LINE;
}

void btLine::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}
