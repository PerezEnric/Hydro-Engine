#include "Application.h"
#include "ModuleUI.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "Json/json.hpp"
#include <fstream>
#include <string>
#include "ImGuizmo/ImGuizmo.h"
#include "GameObject.h"
#include "ModuleCamera3D.h"

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

bool ModuleUI::Init()
{
	

	
	return true;
}

bool ModuleUI::Start()
{
	glewInit();
	LOG("Glew Initilizate");
	ImGui::CreateContext();
	LOG("IMGUI context created");
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	vector_panels.push_back(p_config = new PanelConfig());
	vector_panels.push_back(p_about = new PanelAbout());
	vector_panels.push_back(p_console = new PanelConsole());
	vector_panels.push_back(p_shapes = new PanelShapes());
	vector_panels.push_back(p_hierarchy = new PanelHierarchy());
	vector_panels.push_back(p_inspector = new PanelInspector());
	vector_panels.push_back(p_saveload = new PanelSaveAndLoad());
	vector_panels.push_back(p_game = new PanelGame());

	p_console->is_active = true;
	p_inspector->is_active = true;
	p_game->is_active = true;
	

	return true;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	CreateMainMenuBar();

	ImGui::ShowDemoWindow();

	if (p_hierarchy->show_hierarchy)
		p_hierarchy->is_active;

	if (p_config->show_config_window)
		p_config->is_active;

	if (p_console->show_console)
		p_console->is_active;

	if (p_shapes->show_shapes)
		p_shapes->is_active;

	if (p_about->show_about)
		p_about->is_active;

	if (p_inspector->show_inspector)
		p_inspector->is_active;

	if (logs_to_do.size() > 0)
	{
		for (uint i = 0; i < logs_to_do.size(); i++) {
			p_console->AddLog(logs_to_do[i].c_str());
		}
		logs_to_do.clear();
	}

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

	if (App->scene_intro->selected)
	{
		DrawGuizmos(App->scene_intro->selected);
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

			if (ImGui::MenuItem("Quit")) {
				App->input->quit = true;
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Console", NULL, &p_console->is_active);
			ImGui::MenuItem("Configuration", NULL, &p_config->is_active);
			ImGui::MenuItem("Hierarchy", NULL, &p_hierarchy->is_active);
			ImGui::MenuItem("Inspector", NULL, &p_inspector->is_active);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{
			ImGui::MenuItem("Save & Load", NULL, &p_saveload->is_active);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Create"))
		{
			ImGui::MenuItem("Primitives", NULL, &p_shapes->is_active);
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
			ImGui::MenuItem("About...", NULL, &p_about->is_active);
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

update_status ModuleUI::PostUpdate(float dt)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // look about cycles Guillem.

	return UPDATE_CONTINUE;
}

bool ModuleUI::CleanUp()
{

	for (std::vector<Panel*>::iterator it = vector_panels.begin(); it != vector_panels.end(); ++it)
	{
		if (*it != nullptr)
		{
			delete *it;
			*it = nullptr;
		}
	}
	vector_panels.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->renderer3D->context);
	SDL_DestroyWindow(App->window->window);

	SDL_Quit();
	return true;
}

void ModuleUI::DrawGuizmos(GameObject* selected)
{
	//if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
	//	guizmo_op = ImGuizmo::TRANSLATE;
	//}
	//if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
	//	guizmo_op = ImGuizmo::ROTATE;
	//}
	//if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
	//	guizmo_op = ImGuizmo::SCALE;
	//}

	//float* view_matrix = App->camera->main_cam->GetViewMatrix();
	//float4x4 projection_matrix = App->camera->main_cam->GetProjectionMatrix();

	//float4x4 model = selected->transform->GetGlobalMatrix().Transposed();

	////ImGuizmo::SetDrawlist();
	//ImGuizmo::Manipulate(view_matrix, (float*)& projection_matrix, guizmo_op, guizmo_mode, (float*)& model);


	//if (ImGuizmo::IsUsing)
	//{
	//	model.Transpose();
	//	float3 current_pos = selected->transform->GetPosition();
	//	Quat current_rot = selected->transform->GetRotation();
	//	float3 current_scale = selected->transform->GetScale();
	//	model.Decompose(current_pos, current_rot, current_scale);


	//	switch (guizmo_op)
	//	{
	//	case ImGuizmo::TRANSLATE:
	//		selected->transform->SetPosition(current_pos);
	//		break;

	//	case ImGuizmo::ROTATE:
	//		selected->transform->SetRotationWithQuat(current_rot);
	//		break;

	//	case ImGuizmo::SCALE:
	//		if (current_scale.x > 0.3f && current_scale.y > 0.3f && current_scale.z > 0.3f)
	//		{
	//			selected->transform->SetScale(current_scale);
	//		}
	//		break;
	//	}
	//}

	//ImGuiIO& io = ImGui::GetIO();
	//ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

}
