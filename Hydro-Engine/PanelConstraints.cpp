#include "Application.h"
#include "PanelConstraints.h"
#include "ModulePhysics.h"
#include "ImGui/imgui.h"

PanelConstraints::PanelConstraints()
{
}

PanelConstraints::~PanelConstraints()
{
}

bool PanelConstraints::Update()
{
	bool ret = true;

	if (ImGui::Begin("Constraint point to point", &is_active))
	{
		ImGui::SetWindowPos(ImVec2{ 600, 20 }, ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2{ 200, 350 });

		ImGui::DragFloat3("Anchor A", &anchorA[3], 0.1f, 1.0f, 50.0f);
		ImGui::DragFloat3("Anchor B", &anchorB[3], 0.1f, 1.0f, 50.0f);

		if (ImGui::Button("Create Constraint"))
		{
			CreateConstraint(anchorA, anchorB);
		}
		ImGui::End();
	}

	return ret;
}

void PanelConstraints::CreateConstraint(float3 anchora, float3 anchorb)
{
	btCube c1({ 1.0f, 1.0f, 1.0f });

	c1.SetPos(0.0f, 0.0f, 0.0f);
	c1.my_body = App->physics->AddBody(c1);

	App->scene_intro->list_cubes.push_back(c1);

	btCube c2({ 1.0f, 1.0f, 1.0f });

	c2.SetPos(2.0f, 0.0f, 0.0f);
	c2.my_body = App->physics->AddBody(c2);

	App->scene_intro->list_cubes.push_back(c2);

	App->physics->AddConstraintP2P(*c1.my_body, *c2.my_body, anchora, anchorb);
}
