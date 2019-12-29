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
class GameObject;





class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Start();
	int HowManyMeshes(const std::string& Filename);

	void aiParentNode(const std::string& Filename);

	void NodeIterations(aiNode* parentNod, GameObject* act, bool again = false); //recursive function be carefull

	void ChargeTransform(aiNode* parentNod, GameObject* act); //recursive function be carefull

	void CreateGO(const std::string & Filename, GameObject* act); // we will call this function in the second assigment.

	
	std::string LoadFBX(const std::string& Filename, uint index, GameObject* object);

	std::string LoadTexture(const std::string& Filename, Component_Texture* tex, bool _generating);

	void LoadTexture(const std::string& Filename, Component_Texture* tex);

	void AssetsCarpet(std::string & to_retoc);

	std::string SearchTheDoc(const std::string & Filename, GameObject* tex);
	std::string CutTheDoc(const std::string & Filename, Component_Texture* tex);

	std::string ImportMeshOwnFile(const char* name, Component_Mesh* Mesh);

	std::string ImportTextureOwnFile(const char* name); 

	void ExportMeshOwnFile(const char* pathname, Component_Mesh* Mesh); 

	void ExportTextureOwnFile(const char* pathname, Component_Texture* Tex);

	bool CleanUp();

	bool transforms_done = false;
private:
	std::string local_doc;

};


