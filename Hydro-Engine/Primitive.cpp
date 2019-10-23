#include "Application.h"
#include "Globals.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "Component_Mesh.h"


// ------------------------------------------------------------
Primitive::Primitive()
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::CreatePrimitive(PrimitiveTypes p_type, Component_Mesh* Ret, const float* axis, math::float3 t_vector, float radians, math::float3 s_vector)
{
	par_shapes_mesh* p_mesh = nullptr;

	switch (p_type)
	{
	case PrimitiveTypes::P_CUBE:
		p_mesh = par_shapes_create_cube();
		break;

	case PrimitiveTypes::P_SPHERE:
		p_mesh = par_shapes_create_subdivided_sphere(5);
		break;

	case PrimitiveTypes::P_CYLINDER:
		p_mesh = par_shapes_create_cylinder(20, 5);
		break;

	case PrimitiveTypes::P_PLANE:
		p_mesh = par_shapes_create_plane(20, 20);
		break;
	}

	//Vertex
	Ret->num_vertex = p_mesh->npoints;
	Ret->vertex = new float[Ret->num_vertex * 3];
	memcpy(Ret->vertex, p_mesh->points, Ret->num_vertex * sizeof(float) * 3);
	glGenBuffers(1, (GLuint*) & (Ret->id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, Ret->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * Ret->num_vertex * 3, Ret->vertex, GL_STATIC_DRAW);


	//Index
	Ret->num_index = p_mesh->ntriangles * 3;
	Ret->index = new uint[Ret->num_index];
	memcpy(Ret->index, p_mesh->triangles, Ret->num_index * sizeof(PAR_SHAPES_T));
	glGenBuffers(1, &(Ret->id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ret->id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PAR_SHAPES_T) * Ret->num_index, Ret->index, GL_STATIC_DRAW);


	/*glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawElements(GL_TRIANGLES, p_mesh->ntriangles * 3, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	*/

	/*par_shapes_translate(p_mesh, t_vector.x, t_vector.y, t_vector.z);

	if (axis != nullptr)
	{
		par_shapes_rotate(p_mesh, radians, axis);
	}

	par_shapes_scale(p_mesh, s_vector.x, s_vector.y, s_vector.z);
	*/

	par_shapes_free_mesh(p_mesh);
}
