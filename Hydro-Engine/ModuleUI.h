#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"

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

	bool CleanUp();

private:

	bool show_config_window = false;
	bool show_console = false;
	SDL_WindowFlags window_flags;
};

#endif