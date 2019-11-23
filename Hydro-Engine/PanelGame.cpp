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
	if(ImGui::BeginMenu("Game"))
	{
		if (ImGui::Button("Play"))
		{

		}

		if (ImGui::Button("Pause"))
		{

		}

		if (ImGui::Button("Tick"))
		{

		}
		ImGui::EndMenu();
	}

	
	return true;
}
