#include "Component_Mesh.h"
#include "Glew/include/glew.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Globals.h"
#include "Primitive.h"
#include "ImGui/imgui.h"
#include "MathGeoLib/include/Geometry/AABB.h"




Component_Mesh::Component_Mesh(GameObject* GO, COMPONENT_TYPE type) : Component(GO, type)
{
	if (GO->p_type == PrimitiveTypes::P_NONE) {
		Load_Mesh();
		/*if (GO->actual_mesh < GO->mesh_array)
		{
			GO->actual_mesh++;
		}
		else
			LOG("Error creating meshcomponent");*/
	}
	else
		Load_P_Shape();

	//mesh_bbox.SetNegativeInfinity();
	
	MakeChecker();
}

Component_Mesh::Component_Mesh()
{
}

void Component_Mesh::Load_Mesh()
{
	App->importer->LoadFBX(GO->path, GO->actual_mesh, this);
	//CreateBBox();
}

bool Component_Mesh::Update()
{
	Draw();
	if (show_vertex_normals  && GO->p_type == P_NONE)
		DrawVertexNormals();

	if (show_face_normals && GO->p_type == P_NONE)
		DrawFaceNormals();

	return true;
}

void Component_Mesh::Draw()
{
	glEnable(GL_TEXTURE_2D);
	if (GO->p_type == PrimitiveTypes::P_NONE) // the second parametre is for when the gameobject doesnt have a tex and its not a P_SHAPE.
	{
		if (cheker_tex)
			glBindTexture(GL_TEXTURE_2D, texName);
		if (!cheker_tex && GO->my_tex != nullptr)
			glBindTexture(GL_TEXTURE_2D, GO->my_tex->id_texture);
	}
		

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (GO->p_type == PrimitiveTypes::P_NONE && size != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}
	

	glBindBuffer(GL_ARRAY_BUFFER, id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_index);

	if (GO->p_type == PrimitiveTypes::P_NONE)
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);
	else
		glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_SHORT, NULL);

	glBindTexture(GL_TEXTURE_2D, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
}

void Component_Mesh::DrawVertexNormals()
{
	int lenght = 2;

	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;


	for (uint i = 0; i < num_vertex * 3; i += 3)
	{

		glBegin(GL_LINES);
		glVertex3f(vertex[i], vertex[i + 1], vertex[i + 2]);
		glVertex3f(vertex[i] + normal[i], vertex[i + 1] + normal[i+1]*lenght, vertex[i + 2] + normal[i + 2]*lenght);
		++j;
		glEnd();
	}
	glColor3f(1, 1, 1);

}

void Component_Mesh::DrawFaceNormals()
{
	int l = 2;
	
	for (uint i = 0; i < num_index; i += 3) {
		// Calculo de vectores. ----------------------------------------------------------------------------------
		uint c_i = index[i]*3; // Dentro de la mesh y dentro del cuadrado en la que estamos cojemos el primer indice del cuadrado.
		// Estas son las coordenadas del primer vector.
		float3 a(vertex[c_i], vertex[c_i+1], vertex[c_i+2]);
			
		c_i = index[i + 1]*3; // aqui hacemos que el current index apunte al segundo indice del cuadrado;

		float3 b(vertex[c_i], vertex[c_i + 1], vertex[c_i + 2]);

		c_i = index[i + 2]*3; // aqui hacemos que el current index apunte al tercer indice del cuadrado;

		float3 c(vertex[c_i], vertex[c_i + 1], vertex[c_i + 2]);
		//	Calculo de vectores. ----------------------------------------------------------------------------------
			
		// Calculo del centro del triangulo. -------------------------------------------------------
		float3 tri_cen;
		tri_cen.x = (a.x + b.x + c.x) / 3;
		tri_cen.y = (a.y + b.y + c.y) / 3; // El centro del triangulo lo calculamos sumando todos sus puntos y diviendolo entre 3.
		tri_cen.z = (a.z + b.z + c.z) / 3;
		// Calculo del centro del triangulo. --------------------------------------------------------
			
		// Calculo del plano atraves de 2 vectores. -------------------------------------------------
		float3 a_cen = a - tri_cen;
		float3 b_cen = b - tri_cen; 
		// Ahora tenemos el vector a_cen que va desde el centro hasta el punto "a" y el b_cen que es lo mismo pero con "b".
		float3 orth_vec = a_cen.Cross(b_cen);
		// Calculo del plano atraves de 2 vectores. -------------------------------------------------
		orth_vec.Normalize();
		orth_vec *= l;
			
		glColor3f(1.f, 1.f, 0.25f);
		glBegin(GL_LINES);
		glVertex3f(tri_cen.x, tri_cen.y, tri_cen.z);
		glVertex3f(tri_cen.x + orth_vec.x, tri_cen.y + orth_vec.y, tri_cen.z + orth_vec.z);
		glEnd();
		glColor3f(1, 1, 1);
	}


}

void Component_Mesh::Load_P_Shape()
{
	Primitive P;
	P.CreatePrimitive(GO->p_type,this);
}

void Component_Mesh::CleanUp()
{
	glDeleteBuffers(1, &(id_index));
	glDeleteBuffers(1, &(id_vertex));
	if (GO->p_type != P_NONE)
		glDeleteBuffers(1, &(id_uvs));

	num_index = 0;
	num_vertex = 0;
	size = 0;
	index = nullptr;
	vertex = nullptr;
	text_uvs = nullptr;
	normal = nullptr;

	GO->mesh_array = 0;
	GO->actual_mesh = 0;
	GO->p_type = P_NONE;
	GO->path.clear();


}

Component_Mesh * Component_Mesh::GetThis()
{
	return this;
}

AABB Component_Mesh::CreateBBox()
{
	AABB bbox(float3(0, 0, 0), float3(0, 0, 0));
	mesh_bbox = bbox;
	float3* vertex_array = new float3[num_vertex];

	for (uint i = 0; i < num_vertex; i++)
		vertex_array[i] = float3(vertex[i*3], vertex[i*3 + 1], vertex[i*3 + 2]);

	mesh_bbox.Enclose(vertex_array, num_vertex);


	return mesh_bbox;
}

void Component_Mesh::ShowInfo()
{
	ImGui::Text("Mesh Vertices: %i", num_vertex);
	ImGui::Text("Mesh Indices: %i", num_index);
	ImGui::Text("Mesh Triangles: %i", num_index / 3);

	if (ImGui::Checkbox("Bounding Box", &show_bbox))
	{
		//App->scene_intro->root[App->scene_intro->selected]->CreateBBox();
	}

	if (ImGui::Checkbox("Face Normals", &show_face_normals))
	{
	}

	if (ImGui::Checkbox("Vertices Normals", &show_vertex_normals))
	{
	}

	if (ImGui::Checkbox("checkers texture", &cheker_tex))
	{
	}
}

void Component_Mesh::MakeChecker()
{
	int i, j, c;

	for (i = 0; i < imageheight; i++) {
		for (j = 0; j < imagewidht; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagewidht, imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
	glBindTexture(GL_TEXTURE_2D, NULL);
}
