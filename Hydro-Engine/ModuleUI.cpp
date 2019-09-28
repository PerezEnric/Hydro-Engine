#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
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

	return true;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	CreateMainMenuBar();

	ImGui::ShowDemoWindow();

	if (show_config_window)
		CreateConfigWindow();

	if (show_console)
		CreateConsole();

	if (show_about)
		CreateAbout();

	return UPDATE_CONTINUE;
}

update_status ModuleUI::Update(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleUI::CreateMainMenuBar()
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Console", NULL, &show_console);
			ImGui::MenuItem("Configuration", NULL, &show_config_window);
			

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

void ModuleUI::CreateConfigWindow()
{

	if (ImGui::Begin("Configuration", &show_config_window), window_flags)
	{
		ImGui::SetWindowPos(ImVec2{ 600, 20 }, ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2{ 600, 600 }, ImGuiCond_FirstUseEver);
		if (ImGui::CollapsingHeader("Configuration"))
		{
			
		}

		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL version: %i.%i.%i", App->system_info.sdl_version.major, App->system_info.sdl_version.minor, App->system_info.sdl_version.patch);
			ImGui::Separator();
			ImGui::Text("System Ram: %.2f GB", App->system_info.ram);
			ImGui::Text("CPUs: %i (Cache: %i KB)", App->system_info.cpus, App->system_info.cpu_cache);
			ImGui::Text("Caps:");
			ImGui::SameLine();
			if (App->system_info.has_AVX)
				ImGui::Text("AVX");
			ImGui::SameLine();
			if (App->system_info.has_AVX2)
				ImGui::Text("AVX2");
			ImGui::SameLine();
			if (App->system_info.has_AltiVec)
				ImGui::Text("AltiVec");
			ImGui::SameLine();
			if (App->system_info.has_MMX)
				ImGui::Text("MMX");
			ImGui::SameLine();
			if (App->system_info.has_RDSTC)
				ImGui::Text("RDSTC");
			ImGui::SameLine();
			if (App->system_info.has_3DNow)
				ImGui::Text("3DNow");
			ImGui::SameLine();
			if (App->system_info.has_SSE)
				ImGui::Text("SSE");
			ImGui::SameLine();
			if (App->system_info.has_AVX)
				ImGui::Text("SSE2");
			ImGui::SameLine();
			if (App->system_info.has_AVX)
				ImGui::Text("SSE3");
			ImGui::SameLine();
			if (App->system_info.has_AVX)
				ImGui::Text("SSE41");
			ImGui::SameLine();
			if (App->system_info.has_AVX)
				ImGui::Text("SSE42");
			ImGui::Separator();
			ImGui::Text("GPU vendor: %s", App->system_info.vendor);
			ImGui::Text("GPU brand: %s", App->system_info.renderer);
			ImGui::Text("GPU version: %s", App->system_info.version);
		}
		ImGui::End();
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

	std::ifstream file("Config.json");
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
		about_features.engine_name = j["App"]["Name"].get<std::string>();
		ImGui::Text(about_features.engine_name.c_str());
		about_features.description = j["App"]["Description"].get<std::string>();
		ImGui::Text(about_features.description.c_str());
		about_features.authors = j["App"]["Authors"].get<std::string>();
		ImGui::Text(about_features.authors.c_str());
		about_features.libraries = j["App"]["Libraries"].get<std::string>();
		ImGui::Text(about_features.libraries.c_str());
		about_features.license = j["App"]["License"].get<std::string>();
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
