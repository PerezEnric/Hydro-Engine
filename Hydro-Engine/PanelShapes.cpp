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
		ImGui::SetWindowSize(ImVec2{ 200, 300 });

		ImGui::Text("Cube");
		ImGui::Separator();
		ImGui::DragFloat3("Size", &cube_size[3], 0.1f, 1.0f, 100.0f);

		if (ImGui::Button("Create Cube"))
		{
			//char cube[20];
			//sprintf(cube, "cube %d", current_cube);
			//current_cube++;
			//App->scene_intro->CreateGameObjectPS(cube, PrimitiveTypes::P_CUBE);
			App->scene_intro->CreateCube(cube_size);
		}
		ImGui::Text("Sphere");
		ImGui::Separator();

		ImGui::DragFloat("Radius", &sphere_rad, 0.1f, 1.0f, 100.0f);

		if (ImGui::Button("Sphere"))
		{
			//char sphere[20];
			//sprintf(sphere, "sphere %d", current_sphere);
			//current_sphere++;
			//App->scene_intro->CreateGameObjectPS(sphere, PrimitiveTypes::P_SPHERE);
			App->scene_intro->CreateSphere(sphere_rad);
		}
		ImGui::Text("Cylinder");
		ImGui::Separator();
		ImGui::DragFloat("Rad", &cylinder_rad, 0.1f, 1.0f, 100.0f);
		ImGui::DragFloat("Height", &cylinder_height, 0.1f, 1.0f, 100.0f);
		if (ImGui::Button("Cylinder"))
		{
			//char cylinder[20];
			//sprintf(cylinder, "cylinder %d", current_cylinder);
			//current_cylinder++;
			//App->scene_intro->CreateGameObjectPS(cylinder, PrimitiveTypes::P_CYLINDER);
			App->scene_intro->CreateCylinder(cylinder_rad, cylinder_height);
		}
		ImGui::SameLine();


		ImGui::End();
	}
	
	return ret;
}
