#pragma once

#include "Component.h"
#include "Globals.h"
#include <string>
#include "Component_Texture.h"




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
};