#include "ResourceTexture.h"
#include "Application.h"
#include "Component_Texture.h"
#include "ModuleImporter.h"

ResourceTexture::ResourceTexture(uint uuid, RESOURCE_TYPE type) : Resource(uuid,type)
{
	my_uuid = uuid;
	my_type = type;
}

ResourceTexture::~ResourceTexture()
{
}

bool ResourceTexture::LoadToMemory()
{
	bool ret = false;
	if (loaded == 0)
	{
		my_tex = new Component_Texture();
		App->importer->LoadTexture(path_to_original.c_str(), my_tex); // my own importer doesnt work D:
		if (my_tex == nullptr)
		{
			LOG("Error parsing the Resource texture: %s", path_to_original.c_str());
		}
		else
		{
			LOG("Resource texture: %s loaded correctly", path_to_original.c_str());
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

void ResourceTexture::NotReference()
{
	if (loaded > 0)
		loaded -= 1;
	if (loaded == 0)
		UnLoadFromMemory();
}

bool ResourceTexture::UnLoadFromMemory()
{
	my_tex->CleanResUp();
	loaded = 0;
	return true;
}

void ResourceTexture::CleanUp()
{
	my_tex->CleanResUp();
	loaded = 0;
	my_tex = nullptr;
}
