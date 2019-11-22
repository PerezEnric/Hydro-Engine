#pragma once

#include "Module.h"
#include "Globals.h"
#include "Resource.h"
#include <map>



class Application;

class ResourceManager : public Module
{
public:

	ResourceManager(Application* app, bool start_enabled = true);
	~ResourceManager();

	// This function return the uuid of the resource that we want. < -----------
	uint Find(const char* file_in_assets);									// -
																			// -
	// This function is the first phase to generate a new resource.         // -
	uint ImportFile(const char* path, RESOURCE_TYPE type);					// -
																			// -
	// This function returns the Resource of a uuid is good with this function -
	Resource* Get(uint uuid);


	// Second phase creating a new res.
	Resource* CreateNewRes(uint uuid, RESOURCE_TYPE type);



	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);


private:

	std::map<uint, Resource*> resources;
};



