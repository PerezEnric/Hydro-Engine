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
	my_mesh = new Component_Mesh();
	App->importer->ExportMeshOwnFile(path_to_own.c_str(), my_mesh);
	if (my_mesh != nullptr)
	{
		LOG("Error parsing the mesh");
	}
	else
	{
		ret = true;
		// Tengo que mirar como hacer lo de loaded ghoy.
	}
		
	return ret;
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
