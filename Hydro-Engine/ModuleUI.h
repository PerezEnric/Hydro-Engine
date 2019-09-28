#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include <string>

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
	void CreateConfigWindow();
	void CreateConsole();
	void CreateAbout();

	bool CleanUp();

private:

	bool show_config_window = false;
	bool show_console = false;
	bool show_about = false;

	std::string engine_name;
	std::string description;
	std::string authors;
	SDL_WindowFlags window_flags;
};

#endif