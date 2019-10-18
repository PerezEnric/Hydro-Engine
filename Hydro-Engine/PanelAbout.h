#pragma once

#include "Panel.h"
#include "Json/json.hpp"
#include <fstream>


struct AboutFeatures
{
	std::string engine_name;
	std::string description;
	std::string authors;
	std::string libraries;
	std::string license;
	std::string org;
};

class PanelAbout :public Panel
{
public:
	PanelAbout();
	~PanelAbout();

	bool Update() override;

private:
	AboutFeatures about_features;
	nlohmann::json j;
public:
	bool show_about = false;
};
