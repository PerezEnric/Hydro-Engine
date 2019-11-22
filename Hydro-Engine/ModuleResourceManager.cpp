#include "ModuleResourceManager.h"
#include "PCGRandom/pcg_basic.h"
#include "ResourceMesh.h"
#include "GameObject.h"
#include "Application.h"



ResourceManager::ResourceManager(Application * app, bool start_enabled) : Module(app,start_enabled)
{
}

ResourceManager::~ResourceManager()
{
}

uint ResourceManager::GenerateNewUID()
{
	uint cancer = pcg32_random();
	LOG("%u", cancer);
	return cancer;
}

uint ResourceManager::Find(const char * original_name)
{
		return uint();
}

uint ResourceManager::ImportFile(const char * path, RESOURCE_TYPE type, GameObject* GO)
{
	uint ret = 0;
	bool import_ok = false; 
	std::string written_file;
	std::string imposible = "aogjdakjgnsdjkgsafesaekfpsakfasgkoas";
	
	
	switch (type)
	{
	case R_MESH:
		written_file = App->importer->LoadFBX(path,GO->actual_mesh,GO);
		break;
	}
	if (written_file != imposible) { // If import was successful, create a new resource
		ret = GenerateNewUID();
		CreateNewRes(ret,type,written_file.c_str(),GO);
		
	}
	return ret;
}

Resource * ResourceManager::Get(uint uuid)
{
	std::map<uint, Resource*>::iterator it = resources.find(uuid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

void ResourceManager::CreateNewRes(uint uuid, RESOURCE_TYPE type, const char* path, GameObject* GO)
{
	switch (type) {

	case R_MESH:

		ResourceMesh* ret = nullptr;
		ret = new ResourceMesh(uuid,type); 
		if (ret != nullptr)
		{
			ret->path_to_original = GO->path;
			ret->path_to_own = path;
			ret->original_name = GO->name;
			res_meshes[uuid] = ret;
		}
		break;
	}

}

ResourceMesh * ResourceManager::GetM(uint uuid)
{
	//std::map<uint, ResourceMesh*>::iterator it = res_meshes.find(uuid);
	///*if (it != res_meshes.end())*/

	return res_meshes[uuid];

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
