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
	nlohmann::json j;

	std::ifstream file("About.json");
	if (!file) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		file >> j;
	}

	if (ImGui::Begin("Configuration", &show_config_window), window_flags)
	{
		ImGui::SetWindowPos(ImVec2{ 600, 20 }, ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2{ 600, 600 }, ImGuiCond_FirstUseEver);

		if (ImGui::CollapsingHeader("Application"))
		{
			ImGui::Text("App Name: ");
			ImGui::SameLine();
			about_features.engine_name = j["About"]["Name"].get<std::string>();
			ImGui::Text(about_features.engine_name.c_str());
			ImGui::Text("Organization: ");
			ImGui::SameLine();
			about_features.org = j["About"]["Organization"].get<std::string>();
			ImGui::Text(about_features.org.c_str());

			//FPS and Ms Historigrams

			FillFPSVector();
			FillMsVector();

			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
			ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		}

		if (ImGui::CollapsingHeader("Window Settings"))
		{
			static float i1 = 0;
			ImGui::SliderFloat("Brightness", &i1, 0, 1);
			ImGui::SliderInt("Width", &App->window->width, 640, 1920);
			ImGui::SliderInt("Height", &App->window->height, 480, 1080);
			SDL_SetWindowSize(App->window->window, App->window->width, App->window->height);

			if (ImGui::Checkbox("Fullscreen", &App->window->is_fullscreen))
				App->window->WindowSettings(SDL_WINDOW_FULLSCREEN, App->window->is_fullscreen);
			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &App->window->is_resizable))
				App->window->WindowSettings(SDL_WINDOW_RESIZABLE, App->window->is_resizable);

			if (ImGui::Checkbox("Borderless", &App->window->is_borderless))
				App->window->WindowSettings(SDL_WINDOW_BORDERLESS, App->window->is_borderless);
			ImGui::SameLine();
			if (ImGui::Checkbox("Fullscreen Desktop", &App->window->is_full_desktop))
				App->window->WindowSettings(SDL_WINDOW_FULLSCREEN_DESKTOP, App->window->is_full_desktop);

		}

		if (ImGui::CollapsingHeader("Render Settings"))
		{
			if (ImGui::Checkbox("Depth Test", &App->renderer3D->gl_depth_test))
				App->renderer3D->EnableRenderSettings(GL_DEPTH_TEST, App->renderer3D->gl_depth_test);

			if(ImGui::Checkbox("Cull Face", &App->renderer3D->gl_cull_face))
				App->renderer3D->EnableRenderSettings(GL_CULL_FACE, App->renderer3D->gl_cull_face);

			if(ImGui::Checkbox("Lighting", &App->renderer3D->gl_lighting))
				App->renderer3D->EnableRenderSettings(GL_LIGHTING, App->renderer3D->gl_lighting);

			if (ImGui::Checkbox("Color Material", &App->renderer3D->gl_color_material))
				App->renderer3D->EnableRenderSettings(GL_COLOR_MATERIAL, App->renderer3D->gl_color_material);

			if (ImGui::Checkbox("Texture 2D", &App->renderer3D->gl_texture_2D))
				App->renderer3D->EnableRenderSettings(GL_TEXTURE_2D, App->renderer3D->gl_texture_2D);

			if (ImGui::Checkbox("Blend", &App->renderer3D->gl_blend))
				App->renderer3D->EnableRenderSettings(GL_BLEND, App->renderer3D->gl_blend);

			//TODO2: Make wireframe work
			if (ImGui::Checkbox("Wireframe", &App->renderer3D->is_wireframe))
			{
				if (App->renderer3D->is_wireframe)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				else
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
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
