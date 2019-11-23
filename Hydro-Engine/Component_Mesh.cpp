#include "Component_Mesh.h"
#include "Glew/include/glew.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleResourceManager.h"
#include "ResourceMesh.h"
#include "Resource.h"
#include "Globals.h"
#include "Primitive.h"
#include "ImGui/imgui.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include <vector>


Component_Mesh::Component_Mesh(GameObject* GO, COMPONENT_TYPE type, bool _empty) : Component(GO, type, _empty)
{
	comp_type_str = "mesh";
	if (!_empty)
	{
		uint uuid = App->res_man->FindM(GO->path.c_str()); // 
		if (uuid == 0) // there is nothing like this so it should create a new resource.
		{
			UUID_resource = App->res_man->ImportFile(GO->path.c_str(), RESOURCE_TYPE::R_MESH, GO);
		}
		else
		{
			UUID_resource = uuid;
			App->importer->LoadFBX(GO->path.c_str(), GO->actual_mesh, GO);
		}
			

		GO->my_mesh = this;
		GO->b_mesh = true;

		my_reference = App->res_man->GetM(UUID_resource);
		my_reference->LoadToMemory();

	}
	else
	{
		GO->my_mesh = this;
		GO->b_mesh = true;
	}
	
}

Component_Mesh::Component_Mesh()
{
}

void Component_Mesh::Load_Mesh()
{
	/*App->importer->LoadFBX(GO->path, GO->actual_mesh, this);
	own_file = App->importer->ImportMeshOwnFile(GO->name.c_str(), this); 
	GO->my_mesh = this;
	GO->b_mesh = true;*/
}

bool Component_Mesh::Update()
{
	/*if(GO->cam->DoCulling(GO))*/
	Draw();

	if (show_vertex_normals  && GO->p_type == P_NONE)
		DrawVertexNormals();

	if (show_face_normals && GO->p_type == P_NONE)
		DrawFaceNormals();

	if(show_bbox)
		DrawBBox();
	
	return true;
}

void Component_Mesh::Draw()
{
	
	if (GO->DoIhave(TRANSFORM))
	{
		glPushMatrix();
		glMultMatrixf((const GLfloat *)&GO->transform->my_global_matrix.Transposed());
	}
	
	glEnable(GL_TEXTURE_2D);
	if (GO->p_type == PrimitiveTypes::P_NONE) // the second parametre is for when the gameobject doesnt have a tex and its not a P_SHAPE.
	{
		if (cheker_tex)
			glBindTexture(GL_TEXTURE_2D, texName);
		if (!cheker_tex && GO->my_tex != nullptr)
			glBindTexture(GL_TEXTURE_2D, GO->my_tex->PointerToText());
	}
		

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (GO->p_type == PrimitiveTypes::P_NONE && my_reference->my_mesh->size != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, my_reference->my_mesh->id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}
	

	glBindBuffer(GL_ARRAY_BUFFER, my_reference->my_mesh->id_vertex);
	glVertexPointer(3, GL_FLOAT, 0, NULL);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_reference->my_mesh->id_index);

	if (GO->p_type == PrimitiveTypes::P_NONE)
		glDrawElements(GL_TRIANGLES, my_reference->my_mesh->num_index, GL_UNSIGNED_INT, NULL);
	else
		glDrawElements(GL_TRIANGLES, my_reference->my_mesh->num_index, GL_UNSIGNED_SHORT, NULL);

	glBindTexture(GL_TEXTURE_2D, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	
	if (GO->DoIhave(TRANSFORM))
		glPopMatrix();
}

void Component_Mesh::DrawVertexNormals()
{
	int lenght = 2;

	glColor3f(0.2f, 1.f, 0.25f);
	uint j = 0;


	for (uint i = 0; i < my_reference->my_mesh->num_vertex * 3; i += 3)
	{

		glBegin(GL_LINES);
		glVertex3f(my_reference->my_mesh->vertex[i], my_reference->my_mesh->vertex[i + 1], my_reference->my_mesh->vertex[i + 2]);
		glVertex3f(my_reference->my_mesh->vertex[i] + my_reference->my_mesh->normal[i], my_reference->my_mesh->vertex[i + 1] + my_reference->my_mesh->normal[i+1]*lenght, my_reference->my_mesh->vertex[i + 2] + my_reference->my_mesh->normal[i + 2]*lenght);
		++j;
		glEnd();
	}
	glColor3f(1, 1, 1);

}

void Component_Mesh::DrawFaceNormals()
{
	int l = 2;
	
	for (uint i = 0; i < my_reference->my_mesh->num_index; i += 3) {
		// Vectors ----------------------------------------------------------------------------------
		uint c_i = my_reference->my_mesh->index[i]*3; 		//Inside the mesh and inside the square we are, we get the first index from the square
		// First vector coodrinates
		float3 a(my_reference->my_mesh->vertex[c_i], my_reference->my_mesh->vertex[c_i+1], my_reference->my_mesh->vertex[c_i+2]);
			
		c_i = my_reference->my_mesh->index[i + 1]*3; // current index points to the second index from the square 

		float3 b(my_reference->my_mesh->vertex[c_i], my_reference->my_mesh->vertex[c_i + 1], my_reference->my_mesh->vertex[c_i + 2]);

		c_i = my_reference->my_mesh->index[i + 2]*3; // current index points to the third index from the square

		float3 c(my_reference->my_mesh->vertex[c_i], my_reference->my_mesh->vertex[c_i + 1], my_reference->my_mesh->vertex[c_i + 2]);
		//	Vectors ----------------------------------------------------------------------------------
			
		// Center of the triangle -------------------------------------------------------
		float3 tri_cen;
		tri_cen.x = (a.x + b.x + c.x) / 3;
		tri_cen.y = (a.y + b.y + c.y) / 3; // we sum all the points and the we divide by 3
		tri_cen.z = (a.z + b.z + c.z) / 3; 
		// Center of the triangle --------------------------------------------------------
			
		// Plane of 2 vectors -------------------------------------------------
		float3 a_cen = a - tri_cen;
		float3 b_cen = b - tri_cen; 
		// vector a_cen goes from the center to "a" point and b_cen does the same with "b" point.
		float3 orth_vec = a_cen.Cross(b_cen);
		//  Plane of 2 vectors -------------------------------------------------
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
	// Esto lo tendre que eliminar. gtodo.
	//own_file = App->importer->ImportMeshOwnFile(GO->name.c_str(), this);
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

	GO->my_mesh = nullptr;

	GO->b_mesh = false;

	my_reference->NotReference();
	my_reference = nullptr;


}


Component_Mesh * Component_Mesh::GetThis()
{
	return this;
}

AABB Component_Mesh::CreateAABB()
{
	mesh_bbox.SetNegativeInfinity();
	mesh_bbox.Enclose((float3*)my_reference->my_mesh->vertex, my_reference->my_mesh->num_vertex);

	return mesh_bbox;
}

OBB Component_Mesh::CreateOBB()
{
	OBB obb = CreateAABB();
	obb.Transform(GO->transform->GetGlobalMatrix());

	obb_box.SetNegativeInfinity();
	obb_box.Enclose(obb);
	return obb_box;
}

void Component_Mesh::RecalcBoundingBox()
{
	OBB obb = CreateAABB();
	obb.Transform(App->scene_intro->selected->transform->GetGlobalMatrix());

	obb_box.SetNegativeInfinity();
	obb_box.Enclose(obb);
}

void Component_Mesh::CleanResUp()
{
	glDeleteBuffers(1, &(id_index));
	glDeleteBuffers(1, &(id_vertex));
	glDeleteBuffers(1, &(id_uvs));


	num_index = 0;
	num_vertex = 0;
	size = 0;
	index = nullptr;
	vertex = nullptr;
	text_uvs = nullptr;
	normal = nullptr;
}

nlohmann::json Component_Mesh::SaveComponent()
{
	nlohmann::json ret;

	/// Method 1.
	/*std::vector<float> helper_floats;
	for (uint i = 0; i < num_vertex * 3; i++)
	{
		helper_floats.push_back(vertex[i]);
	}

	ret["Vertex num"] = num_vertex;
	ret["Vertex info"] = helper_floats;*/

	/// Method 2.
	ret["Mesh file"] = own_file.c_str();

	ret["show face normals"] = show_face_normals;

	ret["show vertex normals"] = show_vertex_normals;

	ret["has tex coords"] = Has_tex_coords;

	ret["has normals"] = Has_normals;

	ret["show BBox"] = show_bbox;


	//char* uuid_str = new char[80];

	//sprintf(uuid_str, "%d", UUID_resource);

	ret["My Resource UUID"] = UUID_resource;

	return ret;
}

void Component_Mesh::LoadComponent(nlohmann::json & to_load)
{
	// first we load esential bools and our own file name.

	// Load bools

	show_face_normals = to_load["show face normals"].get<bool>();
	show_vertex_normals = to_load["show vertex normals"].get<bool>();
	Has_tex_coords = to_load["has tex coords"].get<bool>();
	Has_normals = to_load["has normals"].get<bool>(); 
	show_bbox = to_load["show BBox"].get<bool>();

	// Load Strings

	own_file = to_load["Mesh file"].get<std::string>();

	// then we use our importer function to load all vertex data.

	UUID_resource = to_load["My Resource UUID"].get<uint>();

	my_reference = App->res_man->GetM(UUID_resource);
	
	if (my_reference == nullptr)
	{
		uint uuid = App->res_man->FindM(GO->path.c_str());
		if (0 == 0)
		{
			UUID_resource = App->res_man->ImportFile(GO->path.c_str(), RESOURCE_TYPE::R_MESH, GO);
		}
		else
			UUID_resource = uuid;

		my_reference = App->res_man->GetM(UUID_resource);
	}

	my_reference->LoadToMemory();


	

	//App->importer->ExportMeshOwnFile(own_file.c_str(), this);

	//then we create the ABB and the Obb.
	/*if (vertex != nullptr)
		CreateOBB();*/
}

void Component_Mesh::DrawBBox()
{
	glBegin(GL_LINES);

	glLineWidth(30.0f);

	for (uint i = 0; i <= 7; i++)
	{
		glVertex3f(obb_box.CornerPoint(i).x, obb_box.CornerPoint(i).y, obb_box.CornerPoint(i).z);
	}

	glVertex3f(obb_box.CornerPoint(1).x, obb_box.CornerPoint(1).y, obb_box.CornerPoint(1).z);
	glVertex3f(obb_box.CornerPoint(3).x, obb_box.CornerPoint(3).y, obb_box.CornerPoint(3).z);

	glVertex3f(obb_box.CornerPoint(3).x, obb_box.CornerPoint(3).y, obb_box.CornerPoint(3).z);
	glVertex3f(obb_box.CornerPoint(7).x, obb_box.CornerPoint(7).y, obb_box.CornerPoint(7).z);

	glVertex3f(obb_box.CornerPoint(7).x, obb_box.CornerPoint(7).y, obb_box.CornerPoint(7).z);
	glVertex3f(obb_box.CornerPoint(5).x, obb_box.CornerPoint(5).y, obb_box.CornerPoint(5).z);

	glVertex3f(obb_box.CornerPoint(5).x, obb_box.CornerPoint(5).y, obb_box.CornerPoint(5).z);
	glVertex3f(obb_box.CornerPoint(1).x, obb_box.CornerPoint(1).y, obb_box.CornerPoint(1).z);


	glVertex3f(obb_box.CornerPoint(2).x, obb_box.CornerPoint(2).y, obb_box.CornerPoint(2).z);
	glVertex3f(obb_box.CornerPoint(6).x, obb_box.CornerPoint(6).y, obb_box.CornerPoint(6).z);

	glVertex3f(obb_box.CornerPoint(6).x, obb_box.CornerPoint(6).y, obb_box.CornerPoint(6).z);
	glVertex3f(obb_box.CornerPoint(4).x, obb_box.CornerPoint(4).y, obb_box.CornerPoint(4).z);

	glVertex3f(obb_box.CornerPoint(4).x, obb_box.CornerPoint(4).y, obb_box.CornerPoint(4).z);
	glVertex3f(obb_box.CornerPoint(0).x, obb_box.CornerPoint(0).y, obb_box.CornerPoint(0).z);

	glVertex3f(obb_box.CornerPoint(0).x, obb_box.CornerPoint(0).y, obb_box.CornerPoint(0).z);
	glVertex3f(obb_box.CornerPoint(2).x, obb_box.CornerPoint(2).y, obb_box.CornerPoint(2).z);

	glEnd();
}

void Component_Mesh::ShowInfo()
{
	ImGui::Text("Mesh Vertices: %i", num_vertex);
	ImGui::Text("Mesh Indices: %i", num_index);
	ImGui::Text("Mesh Triangles: %i", num_index / 3);

	if (ImGui::Checkbox("Face Normals", &show_face_normals))
	{
	}

	if (ImGui::Checkbox("Vertices Normals", &show_vertex_normals))
	{
	}

	if (ImGui::Checkbox("checkers texture", &cheker_tex))
	{
	}

	if (ImGui::Checkbox("Bounding Box", &show_bbox))
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
	glBindTexture(GL_TEXTURE_2D, NULL); // debind this or get ready to lose the grade.
}
