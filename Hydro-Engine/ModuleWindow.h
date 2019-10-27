#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"
#include "Json/json.hpp"
#include <fstream>

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title);
	void WindowSettings(int win_setting, bool active_set);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;
	SDL_Surface* icon = nullptr;

	bool is_fullscreen = false;
	bool is_resizable = true;
	bool is_borderless = false;
	bool is_full_desktop = false;

	float brightness = 1.0f;
	int width;
	int height;

private: 	
	nlohmann::json j;
};

#endif // __ModuleWindow_H__