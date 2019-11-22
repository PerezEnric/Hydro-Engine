#include "ModuleResourceManager.h"
#include "Application.h"



ResourceManager::ResourceManager(Application * app, bool start_enabled) : Module(app,start_enabled)
{
}

ResourceManager::~ResourceManager()
{
}

uint ResourceManager::Find(const char * file_in_assets)
{
	return uint();
}

uint ResourceManager::ImportFile(const char * path, RESOURCE_TYPE type)
{
	return uint();
}

Resource * ResourceManager::Get(uint uuid)
{
	return nullptr;
}

Resource * ResourceManager::CreateNewRes(uint uuid, RESOURCE_TYPE type)
{
	return nullptr;
}





update_status ResourceManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ResourceManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ResourceManager::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}
