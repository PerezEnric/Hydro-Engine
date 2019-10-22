#pragma once

#include "Module.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/material.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "Glew/include/glew.h"
#include "glmath.h"

class Component_Mesh;


struct Texture {
	GLuint id_texture = 0;
	uint widht = 0;
	uint height = 0;
	uint size = 0;
	float* text_uvs = nullptr;
	uint id_uvs = 0;

};

struct Mesh {

	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	float* normal;
	Texture text_info;
};



class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	int HowManyMeshes(const std::string& Filename);
	
	bool LoadFBX(const std::string& Filename, uint index, Component_Mesh* Ret);

	void LoadTexture(const std::string& Filename);

	void RenderAll();
	void RenderNormals();
	void RenderFaceNormals();

	//AKA cross product;
	vec3 OrthogonalVect(const vec3 a, const vec3 b);

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();
private:
	std::vector<Mesh> _amesh;
	Texture Lenna;
};


