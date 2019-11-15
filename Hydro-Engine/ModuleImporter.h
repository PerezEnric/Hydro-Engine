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
class Component_Texture;





class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	int HowManyMeshes(const std::string& Filename);

	void aiParentNode(const std::string& Filename);

	void NodeIterations(aiNode* parentNod, GameObject* act); //recursive function be carefull

	void CreateGO(const std::string & Filename, GameObject* act); // we will call this function in the second assigment.

	
	bool LoadFBX(const std::string& Filename, uint index, Component_Mesh* Ret);

	void LoadTexture(const std::string& Filename, Component_Texture* tex);


	std::string SearchTheDoc(const std::string & Filename, Component_Texture* tex);
	std::string CutTheDoc(const std::string & Filename, Component_Texture* tex);

	void ImportMeshOwnFile(const char* name, Component_Mesh* Mesh);

	void ImportTextureOwnFile(const char* name); 

	void ExportMeshOwnFile(const char* pathname, Component_Mesh* Mesh); //gtodo

	void ExportTextureOwnFile(); //gtodo no se muy bien como hacerlo xd

	bool CleanUp();
private:
	std::string local_doc;
	int act_number_meshes;
};


