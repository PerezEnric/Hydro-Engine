#include "Component_Texture.h"
#include "Glew/include/glew.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "ModuleResourceManager.h"
#include "ResourceTexture.h"
#include "Globals.h"

Component_Texture::Component_Texture(GameObject * GO, COMPONENT_TYPE type, bool _empty) : Component(GO, type, _empty)
{

	comp_type_str = "texture";
	if (!_empty)
	{
		GO->texture = true;
		GO->my_tex = this;
		GO->just_loading = false;

		uint uuid = App->res_man->FindT(GO->texture_path.c_str()); 
		if (uuid == 0)
		{
			UUID_resource = App->res_man->ImportFile(GO->texture_path.c_str(), RESOURCE_TYPE::R_TEXTURE, GO);
		}
		else
			UUID_resource = uuid;

		

		my_reference = App->res_man->GetT(UUID_resource);
		my_reference->LoadToMemory();


	}
	else
	{
		GO->texture = true;
		GO->my_tex = this;
		GO->just_loading = false;
	}
	
}

Component_Texture::Component_Texture()
{
}

void Component_Texture::Load_Texture()
{
	GO->texture = true;
	App->importer->LoadTexture(GO->texture_path, this, true);
	GO->my_tex = this;
}

Component_Texture * Component_Texture::GetThis()
{
	return this;
}

void Component_Texture::CleanUp()
{
	glDeleteBuffers(1, &(id_texture));
	widht = 0;
	height = 0;
	
	
	GO->texture_path.clear();
	GO->texture = false;
	GO->my_tex = nullptr;

	my_reference->NotReference();
	my_reference = nullptr;
}

void Component_Texture::CleanResUp()
{
	glDeleteBuffers(1, &(id_texture));
	widht = 0;
	height = 0;
}

uint Component_Texture::PointerToText()
{
	if (my_reference != nullptr)
	{
		return my_reference->my_tex->id_texture;
	}
	else
	{
		return id_texture;
	}
	
}

nlohmann::json Component_Texture::SaveComponent()
{
	nlohmann::json compo;

	compo["Own Texture name"] = own_format.c_str();

	

	compo["My Resource UUID"] = UUID_resource;




	return compo;
}

void Component_Texture::LoadComponent(nlohmann::json & to_load)
{
	//Load strings

	own_format = to_load["Own Texture name"].get<std::string>();

	//App->importer->ExportTextureOwnFile(own_format.c_str(), this);
	//App->importer->LoadTexture(GO->texture_path, this, true);


	UUID_resource = to_load["My Resource UUID"].get<uint>();

	my_reference = App->res_man->GetT(UUID_resource);

	if (my_reference == nullptr)
	{
		uint uuid = App->res_man->FindT(GO->texture_path.c_str());
		if (uuid == 0)
		{
			UUID_resource = App->res_man->ImportFile(GO->texture_path.c_str(), RESOURCE_TYPE::R_TEXTURE, GO);
		}
		else
			UUID_resource = uuid;

		my_reference = App->res_man->GetT(UUID_resource);
	}

	my_reference->LoadToMemory();
}

void Component_Texture::ShowInfo()
{

	ImGui::Text("Current Reference: %u", UUID_resource);
	ImGui::Text("Reference Counts: %i", my_reference->loaded);

	ImGui::Text("Texture path: %s", GO->texture_path.c_str());
	ImGui::Text("Texture Size: %ix%i pixels", widht, height);

	if (id_texture == 0) {
		ImGui::Image((ImTextureID)my_reference->my_tex->id_texture, ImVec2(150, 150));
	}
	else {
		ImGui::Image((ImTextureID)my_reference->my_tex->id_texture, ImVec2(150, 150));
	}

	if (ImGui::Button("Delete Texture"))
	{
		GO->EliminateComponent(TEXTURE);
	}
}
