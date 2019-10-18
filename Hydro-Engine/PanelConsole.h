#pragma once

#include "Panel.h"
#include "Imgui/imgui.h"

class PanelConsole : public Panel
{
public:
	PanelConsole();
	virtual ~PanelConsole();

	bool Update() override;

	void Clear();
	void AddLog(const char* entry);

};

