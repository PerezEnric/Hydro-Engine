#pragma once

#include "Component.h"
#include "Globals.h"
#include <string>
#include "Component_Texture.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/include/Geometry/AABB.h"




class Component_Mesh : public Component
{
public:

	Component_Mesh(GameObject* GO,COMPONENT_TYPE type);
	Component_Mesh();
	void Load_Mesh();
	bool Update();
	void Draw();
	void DrawVertexNormals();
	void DrawFaceNormals();
	void Load_P_Shape();

	void CleanUp();

	Component_Mesh* GetThis();
	AABB CreateAABB();
	OBB CreateOBB();

	void ShowInfo();
	AABB mesh_bbox;
public:

	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	float* normal;

	uint size = 0;
	float* text_uvs = nullptr;
	uint id_uvs = 0;

	bool show_face_normals = false;
	bool show_vertex_normals = false;

	bool cheker_tex = false;

	void MakeChecker();

	uint imagewidht = 64;
	uint imageheight = 64;
	GLubyte checkImage[64][64][4];

	GLuint texName;

	bool show_bbox = false;

private:
	
};