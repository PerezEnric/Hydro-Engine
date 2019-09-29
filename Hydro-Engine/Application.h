#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleUI.h"
#include "Glew/include/glew.h"
#include <list>
#include <shellapi.h>

struct HardwareInfo
{
	SDL_version sdl_version;
	int cpus = 0;
	int cpu_cache = 0;
	float ram = 0.0f;
	bool has_3DNow = false;
	bool has_AVX = false;
	bool has_AVX2 = false;
	bool has_AltiVec = false;
	bool has_MMX = false;
	bool has_RDSTC = false;
	bool has_SSE = false;
	bool has_SSE2 = false;
	bool has_SSE3 = false;
	bool has_SSE41 = false;
	bool has_SSE42 = false;
	const GLubyte* vendor = nullptr;
	const GLubyte* renderer = nullptr;
	const GLubyte* version = nullptr;
};


class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleUI* ui;

private:

	Timer	ms_timer;
	float	dt;
	std::list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	void GetSystemInfo();
	update_status Update();
	bool CleanUp();

	void RequestBrowser(const char* url);

public:

	// HARDWARE
	HardwareInfo system_info;

	// FPS

	int frame_count = 0;
	int last_sec_frame_count = 0;
	float avg_fps = 0.0f;
	int prev_last_sec_frame_count = 0;
	int frames_on_last_update = 0;

	Timer frame_time;
	Timer last_sec_frame_time;
	Timer startup_time;

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};