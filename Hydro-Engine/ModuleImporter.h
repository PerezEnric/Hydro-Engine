#pragma once

#include "Module.h"
#include "Assimp/include/cimport.h"
#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#include "Assimp/include/material.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct Texture {
	GLuint id_texture = 0;
	uint widht = 0;
	uint height = 0;
	uint size = 0;
	float* text_uvs = nullptr;

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
	// Con esta funcion cargamos el FBX.
	bool LoadFBX(const std::string& Filename);
	// Una vez esta cargado el fbx (FBXScene) la descomprimimos en las diferentes meshes y las cargamos una a una.
	bool SceneToMesh(const aiScene* FBXScene);
	// Esta es la funcion de carga de meshes, tenemos ademas el scene_meshes_xd que es donde guardaremos cada una de las meshes para luego pintarlas.
	void InitMesh(uint Index, const aiMesh* sMesh);

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
};


