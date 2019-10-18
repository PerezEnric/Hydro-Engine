#pragma once

#include "Panel.h"
#include "Imgui/imgui.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	virtual ~PanelConsole();

	bool Update() override;

	void ClearLog();
	void AddLog(const char* entry);
	static char* m_Strdup(const char* str);
private:
	ImVector<char*> items;
	bool ScrollToBottom = true;
};

