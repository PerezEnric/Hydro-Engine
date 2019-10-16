#include "Application.h"
#include "PanelConfig.h"

#include "ImGui/imconfig.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "Glew/include/glew.h"

#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"

#pragma comment (lib, "Glew/lib/glew32.lib")

PanelConfig::PanelConfig(std::string name): Panel("Configuration")
{

}

PanelConfig::~PanelConfig()
{

}

void PanelConfig::ConfigApplication()
{

}

void PanelConfig::WindowSettings()
{

}

void PanelConfig::RenderSettings()
{

}

void PanelConfig::HardwareInfo()
{
}

bool PanelConfig::Update()
{
	bool ret = true;

	return ret;
}








