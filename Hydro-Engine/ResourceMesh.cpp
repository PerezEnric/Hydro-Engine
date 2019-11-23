#include "Application.h"
#include "ResourceMesh.h"
#include "ModuleImporter.h"
#include "Component_Mesh.h"

ResourceMesh::ResourceMesh(uint uuid, RESOURCE_TYPE type) : Resource(uuid,type)
{
	my_uuid = uuid;
	my_type = type;
}

ResourceMesh::~ResourceMesh()
{
}

bool ResourceMesh::LoadToMemory()
{
	bool ret = false;
	if (loaded == 0)
	{
		my_mesh = new Component_Mesh();
		App->importer->ExportMeshOwnFile(path_to_own.c_str(), my_mesh);
		if (my_mesh == nullptr)
		{
			LOG("Error parsing the Resource mesh %s", path_to_own.c_str());
		}
		else
		{
			LOG("Resource Mesh currently up %s", path_to_own.c_str());
			ret = true;
			loaded += 1;
		}
	}
	else
	{
		loaded += 1;
	}
		
	return ret;
}

void ResourceMesh::NotReference()
{
	if (loaded > 0)
		loaded -= 1;
	if (loaded == 0)
		UnLoadFromMemory();
}

bool ResourceMesh::UnLoadFromMemory()
{
	my_mesh->CleanResUp();
	loaded = 0;
	return true;
}

void ResourceMesh::CleanUp()
{
	my_mesh->CleanResUp();
	loaded = 0;
	my_mesh = nullptr;
	
}
