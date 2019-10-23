#include "Component_Mesh.h"
#include "Glew/include/glew.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Globals.h"
#include "Primitive.h"




Component_Mesh::Component_Mesh(GameObject* GO, COMPONENT_TYPE type) : Component(GO, type)
{
	if (GO->p_type == PrimitiveTypes::P_NONE) {
		if (GO->actual_mesh < GO->mesh_array)
		{
			Load_Mesh();


			GO->actual_mesh++;
		}
		else
			LOG("Error creating meshcomponent");
	}
	else
		Load_P_Shape();
	
}

Component_Mesh::Component_Mesh()
{
}

void Component_Mesh::Load_Mesh()
{
	App->importer->LoadFBX(GO->path, GO->actual_mesh, this);
}

bool Component_Mesh::Update()
{
	Draw();
	return true;
}

void Component_Mesh::Draw()
{
	glEnable(GL_TEXTURE_2D);
	if (GO->my_tex != nullptr)
		glBindTexture(GL_TEXTURE_2D, GO->my_tex->id_texture);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
	glTexCoordPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	if (GO->p_type == PrimitiveTypes::P_NONE)
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);
	else
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_SHORT, NULL);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
}

void Component_Mesh::DrawVertexNormals()
{

}

void Component_Mesh::DrawFaceNormals()
{
}

void Component_Mesh::Load_P_Shape()
{
	Primitive P;
	P.CreatePrimitive(GO->p_type,this);

}
