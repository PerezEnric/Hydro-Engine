#include "Application.h"
#include "PanelGame.h"
#include "PanelSaveAndLoad.h"
#include "ImGui/imgui.h"

PanelGame::PanelGame()
{
}

PanelGame::~PanelGame()
{
}

bool PanelGame::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Game", &is_active, flags);
	
	ImGui::SetWindowPos({ 200, 20.0f });
	ImGui::SetWindowSize(ImVec2(200, 70), ImGuiCond_Always);



	if (ImGui::Button("Play"))
	{
		if (!App->scene_intro->game_t.is_running)
		{
			App->scene_intro->game_t.is_running = true;
			App->scene_intro->SaveScene("auto_play_save");
			App->scene_intro->game_t.Start();
		}
	}
	ImGui::SameLine();
	if (!App->scene_intro->game_t.is_paused)
	{
		if (ImGui::Button("Pause"))
		{
			App->scene_intro->game_t.PauseGame();
		}
	}
	else
	{
		if (ImGui::Button("Resume"))
		{
			App->scene_intro->game_t.ResumeGame();
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop"))
	{
		App->scene_intro->game_t.is_paused = false;
		App->scene_intro->LoadScene("auto_play_save");
		App->scene_intro->game_t.StopGame();
	}


	char t[10];
	sprintf_s(t, 10, "%.2f", App->scene_intro->game_t.ReadSecGame());
	ImGui::Text(t);


	ImGui::End();

	return true;
}
