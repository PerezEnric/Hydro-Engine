#include "Application.h"
#include "ModuleSceneIntro.h"
#include "PanelInspector.h"
#include "Component_Mesh.h"
#include "Component_Texture.h"
#include "Component_Transform.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

PanelInspector::PanelInspector()
{
}

PanelInspector::~PanelInspector()
{
}

bool PanelInspector::Update()
{
	bool ret = true;

	if (ImGui::Begin("Inspector", &is_active, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::SetWindowPos(ImVec2{ 880, 20 }, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2{ 310, 745 }, ImGuiCond_Once);

		if (App->scene_intro->selected != -1)
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				TransformWindow();
			}

			if (ImGui::CollapsingHeader("Mesh"))
			{
				MeshWindow();
			}

			if (ImGui::CollapsingHeader("Texture"))
			{
				TextureWindow();
			}
			if (ImGui::Button("DeleteGameObject"))
			{
				App->scene_intro->DeleteGameObject(App->scene_intro->selected);
			}
		}

		ImGui::End();
	}

	return ret;
}

void PanelInspector::TransformWindow()
{
	App->scene_intro->root[App->scene_intro->selected]->ShowInfo(TRANSFORM);
}

void PanelInspector::MeshWindow()
{
	App->scene_intro->root[App->scene_intro->selected]->ShowInfo(MESH);
}

void PanelInspector::TextureWindow()
{
	App->scene_intro->root[App->scene_intro->selected]->ShowInfo(TEXTURE);
}