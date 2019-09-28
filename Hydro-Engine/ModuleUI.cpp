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

	if (ImGui::BeginPopup("PopUp"))
	{
		nlohmann::json j;

		std::ifstream i("Config.json");
		if (!i) {
			LOG("Could not open config_file");
		}
		else {
			LOG("Config_file succesfully loaded");
			i >> j;
		}

		std::string name;
		name = j["App"]["Name"].get<std::string>();
		ImGui::Text(name.c_str());
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
