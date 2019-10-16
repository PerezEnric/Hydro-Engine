#pragma once

#include "Panel.h"


class PanelConfig :public Panel
{
public:
	PanelConfig(std::string name);
	~PanelConfig();

	bool Update();

	void ConfigApplication();
	void WindowSettings();
	void RenderSettings();
	void HardwareInfo();

};
