#include "Application.h"
#include "PanelHierarchy.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ImGui/imgui.h"


PanelHierarchy::PanelHierarchy() : Panel()
{
	is_active = true;
}

PanelHierarchy::~PanelHierarchy()
{
}

bool PanelHierarchy::Update()
{
	bool ret = true;

	if (ImGui::Begin("Hierarchy", &is_active))
	{
		ImGui::SetWindowPos(ImVec2{ 0, 20 }, ImGuiCond_Once);
		ImGui::SetWindowSize(ImVec2{ 200, 600 }, ImGuiCond_Once);

		CreateNode();

		ImGui::End();
	}

	return ret;
}

void PanelHierarchy::CreateNode()
{
	if (ImGui::TreeNode("GameObjects:"))
	{
		for (int n = 0; n < App->scene_intro->root.size(); n++)
		{
			if (ImGui::Selectable(App->scene_intro->root[n]->name.c_str(), App->scene_intro->selected == n))
				App->scene_intro->selected = n;
		}
		ImGui::TreePop();
	}
}