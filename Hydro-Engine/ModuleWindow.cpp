#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include <fstream>
#include "Json/json.hpp"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	icon = SDL_LoadBMP("Assets/hydro_icon.bmp");

	std::ifstream file("Config.json");
	if (!file) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		file >> j;
	}

	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		width = j["Config"]["Window"]["Width"].get<int>() * SCREEN_SIZE;
		height = j["Config"]["Window"]["Height"].get<int>() * SCREEN_SIZE;

		is_fullscreen = j["Config"]["Window"]["Flags"]["Fullscreen"].get<bool>();
		is_resizable = j["Config"]["Window"]["Flags"]["Resizable"].get<bool>();
		is_borderless = j["Config"]["Window"]["Flags"]["Borderless"].get<bool>();
		is_full_desktop = j["Config"]["Window"]["Flags"]["Fulldesktop"].get<bool>();
		
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(is_fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(is_resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(is_borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(is_full_desktop)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
			SDL_SetWindowIcon(window, icon);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	if (icon != NULL)
	{
		SDL_FreeSurface(icon);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::WindowSettings(int win_setting, bool active_set)
{
	if (active_set)
	{
		if(win_setting == SDL_WINDOW_FULLSCREEN_DESKTOP)
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);

		if(win_setting == SDL_WINDOW_FULLSCREEN)
			SDL_SetWindowFullscreen(App->window->window, SDL_WINDOW_FULLSCREEN);

		if (win_setting == SDL_WINDOW_BORDERLESS)
			SDL_SetWindowBordered(App->window->window, (SDL_bool)false);

		if (win_setting == SDL_WINDOW_RESIZABLE)
			SDL_SetWindowResizable(window, (SDL_bool)true);
	}

	else
	{
		SDL_SetWindowBordered(App->window->window, (SDL_bool)true);
		SDL_SetWindowResizable(window, (SDL_bool)false);
		SDL_SetWindowFullscreen(window, 0);
	}
		
}
