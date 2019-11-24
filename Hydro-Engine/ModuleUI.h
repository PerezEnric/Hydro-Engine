#ifndef __ModuleUI_H__
#define __ModuleUI_H__

#include "Module.h"
#include "Primitive.h"
#include "PanelConfig.h"
#include "PanelAbout.h"
#include "PanelConsole.h"
#include "PanelShapes.h"
#include "PanelHierarchy.h"
#include "PanelInspector.h"
#include "PanelSaveAndLoad.h"
#include "PanelGame.h"
#include "PanelCamera.h"
#include "ImGuizmo/ImGuizmo.h"
#include <string>
#include <vector>
#include "Json/json.hpp"
#include <fstream>

class Panel;
class PanelConfig;
class PanelAbout;
class PanelConsole;
class PanelShapes;
class PanelHierarchy;
class PanelInspector;
class PanelSaveAndLoad;
class PanelGame;
class PanelCamera;

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void CreateMainMenuBar();
	void DrawGuizmos(GameObject* selected);

	bool CleanUp();

public:
	PanelConfig* p_config = nullptr;
	PanelAbout* p_about = nullptr;
	PanelConsole* p_console = nullptr;
	PanelShapes* p_shapes = nullptr;
	PanelHierarchy* p_hierarchy = nullptr;
	PanelInspector* p_inspector = nullptr;
	PanelSaveAndLoad* p_saveload = nullptr;
	PanelGame* p_game = nullptr;
	PanelCamera* p_camera = nullptr;
	
	std::vector<Panel*> vector_panels;

	std::vector<std::string> logs_to_do;

private:

	par_shapes_mesh* _mesh = nullptr;
	Primitive c;
	ImGuizmo::OPERATION guizmo_op;
	ImGuizmo::MODE guizmo_mode = ImGuizmo::MODE::WORLD;

	float4x4 Last_model;
	std::string Last_mod;
};

#endif