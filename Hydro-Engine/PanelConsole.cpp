#include "Application.h"
#include "ModuleInput.h"
#include "PanelConsole.h"
#include "ImGuiConsole.h"


// ---------------------------------------------------------
PanelConsole::PanelConsole() : Panel()
{

}

// ---------------------------------------------------------
PanelConsole::~PanelConsole()
{}

// ---------------------------------------------------------
void PanelConsole::ClearLog()
{
	for (int i = 0; i < items.Size; i++)
		free(items[i]);
	items.clear();
}

// ---------------------------------------------------------
void PanelConsole::AddLog(const char* entry)
{
	char buf[1024];
	va_list args;
	va_start(args, entry);
	vsnprintf(buf, IM_ARRAYSIZE(buf), entry, args);
	buf[IM_ARRAYSIZE(buf) - 1] = 0;
	va_end(args);
	items.push_back(m_Strdup(buf));
}

char* PanelConsole::m_Strdup(const char* str)
{
	size_t len = strlen(str) + 1; 
	void* buf = malloc(len); 
	IM_ASSERT(buf); 
	return (char*)memcpy(buf, (const void*)str, len);
}

// ---------------------------------------------------------
bool PanelConsole::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Console", &is_active, flags);

	ImGui::SetWindowPos(ImVec2(0, 765));
	ImGui::SetWindowSize(ImVec2(App->window->width, 250));


	ImGui::End();

	return true;
}