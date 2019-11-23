#include "Application.h"
#include "PanelGame.h"
#include "ImGui/imgui.h"

PanelGame::PanelGame()
{
}

PanelGame::~PanelGame()
{
}

bool PanelGame::Update()
{
	if(ImGui::Begin("Game"))
	{
		ImGui::SetWindowPos({ 200, 20.0f });
		ImGui::SetWindowSize(ImVec2(200, 70), ImGuiCond_Always);

		if (ImGui::Button("Play"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{

		}
		ImGui::SameLine();
		if (ImGui::Button("Tick"))
		{

		}
		ImGui::End();
	}

	
	return true;
}
