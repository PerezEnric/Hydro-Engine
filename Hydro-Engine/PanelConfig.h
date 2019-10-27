#pragma once

#include "Panel.h"
#include <vector>
#include "Json/json.hpp"
#include <fstream>

class PanelConfig :public Panel
{
public:
	PanelConfig();
	~PanelConfig();

	bool Update() override;
	
	void ConfigApplication();
	void WindowSettings();
	void RenderSettings();
	void HardwareInfo();
	void InputInfo();

	void FillFPSVector();
	void FillMsVector();
	void FillMemVector();

private:
	std::vector<float> fps_log;
	std::vector<float> ms_log;
	std::vector<float> mem_log;

	std::string engine_name;
	std::string organization;
	//sMStats stats = m_getMemoryStatistics();
	nlohmann::json j;

public:
	bool show_config_window = false;
};


