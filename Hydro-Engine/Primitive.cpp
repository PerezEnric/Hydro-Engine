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
void Primitive::CreatePrimitive(PrimitiveTypes p_type, const float* axis, math::float3 t_vector, float radians, math::float3 s_vector)
{

	switch (p_type)
	{
	case PrimitiveTypes::P_CUBE:
		mesh = par_shapes_create_cube();
		break;

	case PrimitiveTypes::P_SPHERE:
		mesh = par_shapes_create_subdivided_sphere(5);
		break;

	case PrimitiveTypes::P_CYLINDER:
		mesh = par_shapes_create_cylinder(20, 5);
		break;

	case PrimitiveTypes::P_PLANE:
		mesh = par_shapes_create_plane(20, 20);
		break;
	}

	glGenBuffers(1, (GLuint*) & (m_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->npoints * 3, mesh->points, GL_STATIC_DRAW);
	glGenBuffers(1, &(m_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * mesh->ntriangles * 3, mesh->triangles, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);

	par_shapes_translate(mesh, t_vector.x, t_vector.y, t_vector.z);
	par_shapes_rotate(mesh, radians, axis);
	par_shapes_scale(mesh, s_vector.x, s_vector.y, s_vector.z);


	par_shapes_free_mesh(mesh);
}
