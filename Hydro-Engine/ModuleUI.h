#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Primitive.h"
#include "PanelConfig.h"
#include <string>
#include <vector>
#include "Json/json.hpp"
#include <fstream>

class Panel;
class PanelConfig;

struct AboutFeatures
{
	std::string engine_name;
	std::string description;
	std::string authors;
	std::string libraries;
	std::string license;
	std::string org;
};

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void CreateMainMenuBar();
	void CreateConsole();
	void CreateAbout();
	void FillFPSVector();
	void FillMsVector();

	bool CleanUp();

public:
	PanelConfig* p_config = nullptr;
	std::vector<Panel*> vector_panels;

private:

	bool show_console = false;
	bool show_about = false;

	std::vector<float> fps_log;
	std::vector<float> ms_log;

	AboutFeatures about_features;

	nlohmann::json j;

	std::ifstream file;
	par_shapes_mesh* _mesh = nullptr;
	Primitive c;
};

#endif