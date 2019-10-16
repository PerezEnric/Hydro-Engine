#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "Json/json.hpp"
#include <fstream>
#include <string>

#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGuiConsole.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Glew/include/glew.h"

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"

#pragma comment (lib, "Glew/lib/glew32.lib")

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	glewInit();
	
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	std::ifstream file("About.json");
	if (!file) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		file >> j;
	}

	vector_panels.push_back(p_config = new PanelConfig("Configuration"));

	return true;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	CreateMainMenuBar();

	ImGui::ShowDemoWindow();

	if (p_config->show_config_window)
		p_config->is_active;


	if (show_console)
		CreateConsole();

	if (show_about)
		CreateAbout();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{
	for (std::vector<Panel*>::iterator i = vector_panels.begin(); i != vector_panels.end(); ++i)
	{
		if ((*i)->IsActive()) {
			(*i)->Update();
		}
	}
	return UPDATE_CONTINUE;
}

void ModuleUI::CreateMainMenuBar()
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save")) {
				App->SaveConfig();
			}

			if (ImGui::MenuItem("Load")) {
				App->LoadConfig();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Console", NULL, &show_console);
			ImGui::MenuItem("Configuration", NULL, &p_config->is_active);
			

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
			{
				ImGui::ShowDemoWindow();
			}

			if (ImGui::MenuItem("Documentation"))
			{
				App->RequestBrowser("https://github.com/PerezEnric/Hydro-Engine/wiki");
			}
			if (ImGui::MenuItem("Download latest"))
			{
				App->RequestBrowser("https://github.com/PerezEnric/Hydro-Engine/releases");
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				App->RequestBrowser("https://github.com/PerezEnric/Hydro-Engine/issues");
			}
			ImGui::MenuItem("About...", NULL, &show_about);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ModuleUI::CreateConsole()
{
	ImGuiConsole console;
	console.Draw("console", &show_console);
}

void ModuleUI::CreateAbout()
{
	ImGui::OpenPopup("PopUp");

	nlohmann::json j;

	std::ifstream file("About.json");
	if (!file) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		file >> j;
	}

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
			show_about = false;
		}
		ImGui::EndPopup();
	}
}

void ModuleUI::FillFPSVector()
{
	if (fps_log.size() < 100)
	{
		for (uint i = 0; fps_log.size() < 100; i++)
		{
			fps_log.push_back(App->GetFPS());
		}
	}

	else
		fps_log.erase(fps_log.begin());
}

void ModuleUI::FillMsVector()
{
	if (ms_log.size() < 100)
	{
		for (uint i = 0; ms_log.size() < 100; i++)
		{
			ms_log.push_back(App->GetMs());
		}
	}

	else
		ms_log.erase(ms_log.begin());
}

update_status ModuleUI::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();
	return true;
}
