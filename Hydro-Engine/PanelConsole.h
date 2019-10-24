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
	void AddLog(const char* entry, ...) IM_FMTARGS(2);
	static char* m_Strdup(const char* str);
private:
	ImVector<char*> items;
	bool ScrollToBottom = true;

public:
	bool show_console = true;
};

