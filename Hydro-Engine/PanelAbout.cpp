#include "PanelAbout.h"
#include "Application.h"

#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGuiConsole.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Glew/include/glew.h"

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"

#pragma comment (lib, "Glew/lib/glew32.lib")

PanelAbout::PanelAbout(): Panel()
{
	std::ifstream file("About.json");
	if (!file) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		file >> j;
	}
}

PanelAbout::~PanelAbout()
{
}

bool PanelAbout::Update()
{
	ImGui::OpenPopup("PopUp");

	if (ImGui::BeginPopupModal("PopUp"))
	{
		ImGui::Text("About...");
		ImGui::Separator();
		about_features.engine_name = j["About"]["Name"].get<std::string>();
		ImGui::Text(about_features.engine_name.c_str());
		about_features.description = j["About"]["Description"].get<std::string>();
		ImGui::Text(about_features.description.c_str());
		about_features.authors = j["About"]["Authors"].get<std::string>();
		ImGui::Text(about_features.authors.c_str());
		about_features.libraries = j["About"]["Libraries"].get<std::string>();
		ImGui::Text(about_features.libraries.c_str());
		about_features.license = j["About"]["License"].get<std::string>();
		ImGui::Text(about_features.license.c_str());
		ImGui::NewLine();

		if (ImGui::Button("Close"))
		{
			ImGui::CloseCurrentPopup();
			is_active = false;
		}
		ImGui::EndPopup();
	}

	return true;
}
