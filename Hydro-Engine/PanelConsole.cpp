#include "Application.h"
#include "ModuleInput.h"
#include "PanelConsole.h"


// ---------------------------------------------------------
PanelConsole::PanelConsole() : Panel("Console")
{

}

// ---------------------------------------------------------
PanelConsole::~PanelConsole()
{}

// ---------------------------------------------------------
void PanelConsole::Clear()
{
}

// ---------------------------------------------------------
void PanelConsole::AddLog(const char* entry)
{

}

// ---------------------------------------------------------
bool PanelConsole::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove;
	ImGui::Begin("Console", &is_active, flags);

	ImGui::SetWindowPos(ImVec2(0, 765));
	ImGui::SetWindowSize(ImVec2(App->window->width, 250));


	ImGui::End();

	return true;
}