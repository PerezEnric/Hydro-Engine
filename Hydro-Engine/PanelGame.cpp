#include "Application.h"
#include "PanelGame.h"
#include "PanelSaveAndLoad.h"
#include "ModulePhysics.h"
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
	ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_Always);



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
		App->scene_intro->game_t.is_running = false;
		App->scene_intro->game_t.we_stoped = true;
		App->physics->SupaCleanUp();
		App->scene_intro->LoadScene("Library/Scenes/auto_play_save.json");
		App->scene_intro->game_t.StopGame();

	}


	char t[25];
	sprintf_s(t, 25, "GameSeconds %.2f", App->scene_intro->game_t.ReadSecGame());
	ImGui::Text(t);

	char r_t[25];
	sprintf_s(r_t, 25, "RealTime %.2f", App->startup_time.ReadSec());
	ImGui::Text(r_t);

	ImGui::End();

	return true;
}
