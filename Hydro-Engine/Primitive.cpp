#include "Application.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"


// ------------------------------------------------------------
Primitive::Primitive()
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() 
{
	mesh = par_shapes_create_cube();


	glGenBuffers(1, &m_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->npoints * 3, mesh->points, GL_STATIC_DRAW);

	glGenBuffers(1, &m_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * shape_indices * 3, mesh->triangles, GL_STATIC_DRAW);

	par_shapes_scale(mesh, 1, 1, 1);
	par_shapes_free_mesh(mesh);
}

void Primitive::InnerRender() const
{
}
// ------------------------------------------------------------

// CUBE ============================================

Cube::Cube(): Primitive()
{
	//mesh = par_shapes_create_cube();
}

Cube::~Cube()
{
	
}

void Cube::InnerRender()
{
	par_shapes_mesh* mesh = par_shapes_create_cube();
	shape_indices = mesh->ntriangles;
	par_shapes_free_mesh(mesh);
}
// SPHERE ============================================



// CYLINDER ============================================



// PLANE ==================================================
