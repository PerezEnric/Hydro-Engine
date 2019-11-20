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
	}
	else
		Load_P_Shape();

	
	MakeChecker();
}

Component_Mesh::Component_Mesh()
{
}

void Component_Mesh::Load_Mesh()
{
	App->importer->LoadFBX(GO->path, GO->actual_mesh, this);
	GO->my_mesh = this;
}

bool Component_Mesh::Update()
{
	if(GO->cam->DoCulling(GO))
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
	
	if (GO->DoIhave(TRANSFORM))
		glPopMatrix();
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
		// Vectors ----------------------------------------------------------------------------------
		uint c_i = index[i]*3; 		//Inside the mesh and inside the square we are, we get the first index from the square
		// First vector coodrinates
		float3 a(vertex[c_i], vertex[c_i+1], vertex[c_i+2]);
			
		c_i = index[i + 1]*3; // current index points to the second index from the square 

		float3 b(vertex[c_i], vertex[c_i + 1], vertex[c_i + 2]);

		c_i = index[i + 2]*3; // current index points to the third index from the square

		float3 c(vertex[c_i], vertex[c_i + 1], vertex[c_i + 2]);
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

AABB Component_Mesh::CreateAABB()
{
	mesh_bbox.SetNegativeInfinity();
	mesh_bbox.Enclose((float3*)vertex, num_vertex);

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
