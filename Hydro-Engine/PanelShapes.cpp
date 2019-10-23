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
			App->scene_intro->CreateGameObjectPS("cube", PrimitiveTypes::P_CUBE);
		}
		ImGui::SameLine();
		if (ImGui::Button("Sphere"))
		{
			App->scene_intro->CreateGameObjectPS("sphere", PrimitiveTypes::P_SPHERE);
		}
		if (ImGui::Button("Cylinder"))
		{
			App->scene_intro->CreateGameObjectPS("cylinder", PrimitiveTypes::P_CYLINDER);
		}
		ImGui::SameLine();
		if (ImGui::Button("Plane"))
		{
			App->scene_intro->CreateGameObjectPS("plane", PrimitiveTypes::P_PLANE);
		}


		ImGui::End();
	}
	
	return ret;
}
