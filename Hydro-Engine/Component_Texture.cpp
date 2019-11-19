#include "Component_Texture.h"
#include "Glew/include/glew.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleImporter.h"
#include "Globals.h"

Component_Texture::Component_Texture(GameObject * GO, COMPONENT_TYPE type) : Component(GO, type)
{
	comp_type_str = "texture";
	if (!GO->texture)
		Load_Texture();
	else
		LOG("Error: this gameobject alredy have a texture");
}

Component_Texture::Component_Texture()
{
}

void Component_Texture::Load_Texture()
{
	GO->texture = true;
	App->importer->LoadTexture(GO->texture_path, this);
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
}

nlohmann::json Component_Texture::SaveComponent()
{
	nlohmann::json compo;

	compo["Own Texture name"] = own_format.c_str();

	char* uuid_str = new char[80];

	sprintf(uuid_str, "%d", GO->my_uuid);

	compo["My parent UUID"] = uuid_str;

	return compo;
}

void Component_Texture::ShowInfo()
{
	ImGui::Text("Texture path: %s", GO->texture_path.c_str());
	ImGui::Text("Texture Size: %ix%i pixels", widht, height);

	if (id_texture == 0) {
		ImGui::Image((ImTextureID)id_texture, ImVec2(150, 150));
	}
	else {
		ImGui::Image((ImTextureID)id_texture, ImVec2(150, 150));
	}

	if (ImGui::Button("Delete Texture"))
	{
		GO->EliminateComponent(TEXTURE);
	}
}
