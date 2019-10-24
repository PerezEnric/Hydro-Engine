#include "PanelInspector.h"
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



		ImGui::End();
	}

	return ret;
}
