#include "Application.h"
#include "PanelSaveAndLoad.h"
#include "ImGui/imgui.h"
#include "ModuleFileSystem.h"
#include "ModuleSceneIntro.h"
#include "WindowsItem.h"


PanelSaveAndLoad::PanelSaveAndLoad()
{
}

PanelSaveAndLoad::~PanelSaveAndLoad()
{
}

bool PanelSaveAndLoad::Update()
{
	if (ImGui::Begin("Scene: Save Load", &is_active))
	{
		ImGui::SetWindowPos(ImVec2{ 600, 20 }, ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2{ 200, 100 });
		
		ImGui::InputTextWithHint("","name the scene", str1, IM_ARRAYSIZE(str1));

		if (ImGui::Button("Save"))
		{
			SaveScene();
		}
		
		if (ImGui::Button("Load"))
		{
			std::string file_path = openFileWID();
			
			LoadScene(file_path);
		}
		ImGui::End();
	}
	return true;
}

void PanelSaveAndLoad::SaveScene()
{
	std::string output = str1;
	App->scene_intro->SaveScene(output);
}

void PanelSaveAndLoad::LoadScene(std::string path)
{

	App->scene_intro->LoadScene(path);
}

