#pragma once

#include "Module.h"
#include "Globals.h"
#include "Resource.h"
#include <map>



class Application;
class GameObject;
class ResourceMesh;

class ResourceManager : public Module
{
public:

	ResourceManager(Application* app, bool start_enabled = true);
	~ResourceManager();
	uint GenerateNewUID();
	// This function return the uuid of the resource that we want. < -----------
	uint Find(const char* original_name);									// -
																			// -
	// This function is the first phase to generate a new resource.         // -
	uint ImportFile(const char* path, RESOURCE_TYPE type, GameObject* object);   // -
																			// -
	// This function returns the Resource of a uuid is good with this function -
	Resource* Get(uint uuid); 


	// Second phase creating a new res.
	void CreateNewRes(uint uuid, RESOURCE_TYPE type, const char* path, GameObject* object);

	ResourceMesh* GetM(uint uuid);

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);


private:

	std::map<uint, Resource*> resources;

	std::map<uint, ResourceMesh*> res_meshes;
};



