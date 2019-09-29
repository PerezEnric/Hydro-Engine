#include "Application.h"
#include "SDL/include/SDL_cpuinfo.h"
#include <list>


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	ui = new ModuleUI(this);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	
	// Scenes
	AddModule(scene_intro);

	//UI
	AddModule(ui);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end())
	{
		delete *item;
		item++;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();
	

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (*item)->Start();
		item++;
	}
	
	GetSystemInfo();

	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	frame_count++;
	last_sec_frame_count++;
	frame_time.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (last_sec_frame_time.Read() > 1000)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}

	avg_fps = frame_count / startup_time.ReadSec();
	int last_frame_ms = frame_time.Read();
	frames_on_last_update = prev_last_sec_frame_count;

	LOG("%i", frames_on_last_update);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::reverse_iterator item = list_modules.rbegin();

	while(item != list_modules.rend() && ret == true)
	{
		ret = (*item)->CleanUp();
		item++;
	}
	return ret;
}

void Application::RequestBrowser(const char* url)
{
	ShellExecute(GetActiveWindow(), "open", url, nullptr, nullptr, SW_SHOWNORMAL);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::GetSystemInfo()
{
	SDL_GetVersion(&system_info.sdl_version);
	system_info.cpus = SDL_GetCPUCount();
	system_info.cpu_cache = SDL_GetCPUCacheLineSize();
	system_info.ram = SDL_GetSystemRAM() * 0.001;

	system_info.has_AVX = SDL_HasAVX();
	system_info.has_AVX2 = SDL_HasAVX2();
	system_info.has_AltiVec = SDL_HasAltiVec();
	system_info.has_MMX = SDL_HasMMX();
	system_info.has_3DNow = SDL_Has3DNow();
	system_info.has_RDSTC = SDL_HasRDTSC();
	system_info.has_SSE = SDL_HasSSE();
	system_info.has_SSE2 = SDL_HasSSE2();
	system_info.has_SSE3 = SDL_HasSSE3();
	system_info.has_SSE41 = SDL_HasSSE41();
	system_info.has_SSE42 = SDL_HasSSE42();

	system_info.vendor = glGetString(GL_VENDOR);
	system_info.renderer = glGetString(GL_RENDERER);
	system_info.version = glGetString(GL_VERSION);
}