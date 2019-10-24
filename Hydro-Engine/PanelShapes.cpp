#include "Application.h"
#include "PanelShapes.h"
#include "ImGui/imgui.h"
#include "ModuleSceneIntro.h"

PanelShapes::PanelShapes() : Panel()
{
}

PanelShapes::~PanelShapes()
{
}

bool PanelShapes::Update()
{
	bool ret = true;

	if (ImGui::Begin("Primitive shapes", &is_active))
	{
		ImGui::SetWindowPos(ImVec2{ 600, 20 }, ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2{ 200, 100 });

		if (ImGui::Button("Cube"))
		{
			char cube[20];
			sprintf(cube, "cube %d", current_cube);
			current_cube++;
			App->scene_intro->CreateGameObjectPS(cube, PrimitiveTypes::P_CUBE);
			
		}
		ImGui::SameLine();
		if (ImGui::Button("Sphere"))
		{
			char sphere[20];
			sprintf(sphere, "sphere %d", current_sphere);
			current_sphere++;
			App->scene_intro->CreateGameObjectPS(sphere, PrimitiveTypes::P_SPHERE);
		}
		if (ImGui::Button("Cylinder"))
		{
			char cylinder[20];
			sprintf(cylinder, "cylinder %d", current_cylinder);
			current_cylinder++;
			App->scene_intro->CreateGameObjectPS(cylinder, PrimitiveTypes::P_CYLINDER);
		}
		ImGui::SameLine();
		if (ImGui::Button("Plane"))
		{
			char plane[20];
			sprintf(plane, "plane %d", current_plane);
			current_plane++;
			App->scene_intro->CreateGameObjectPS(plane, PrimitiveTypes::P_PLANE);
		}


		ImGui::End();
	}
	
	return ret;
}
