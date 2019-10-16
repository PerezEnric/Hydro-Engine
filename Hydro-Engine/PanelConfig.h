#pragma once

#include "Panel.h"
#include <vector>

#include "Json/json.hpp"
#include <fstream>

class PanelConfig :public Panel
{
public:
	PanelConfig(std::string name);
	~PanelConfig();

	bool Update() override;
	
	void ConfigApplication();
	void WindowSettings();
	void RenderSettings();
	void HardwareInfo();

	void FillFPSVector();
	void FillMsVector();

private:
	std::vector<float> fps_log;
	std::vector<float> ms_log;

	std::string engine_name;
	std::string organization;

	nlohmann::json j;
	//SDL_WindowFlags window_flags;

public:
	bool show_config_window = false;
};


