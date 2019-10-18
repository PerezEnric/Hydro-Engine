#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Primitive.h"
#include "PanelConfig.h"
#include "PanelAbout.h"
#include "PanelConsole.h"
#include <string>
#include <vector>
#include "Json/json.hpp"
#include <fstream>

class Panel;
class PanelConfig;
class PanelAbout;
class PanelConsole;

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

	bool CleanUp();

public:
	PanelConfig* p_config = nullptr;
	PanelAbout* p_about = nullptr;
	PanelConsole* p_console = nullptr;
	std::vector<Panel*> vector_panels;

private:

	bool show_console = false;
	par_shapes_mesh* _mesh = nullptr;
	Primitive c;
};

#endif