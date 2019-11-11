#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Json/json.hpp"
#include "GameObject.h"
#include <fstream>
#include <istream>
#include <string>





ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	nlohmann::json j;
	
	std::ifstream i("Config.json");
	if (!i) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		i >> j;
	}

	std::string name;
	name = j["Config"]["App"]["Name"].get<std::string>();
	App->window->SetTitle(name.c_str());

	

	return ret;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	if (!house_loaded)
	{
		if (second_cycle)
		{
			CreateGameObject("house", "Assets/BakerHouse.fbx");
			house_loaded = true;
		}
		second_cycle = true;
		
	}
	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::CleanUp()
{
	if (!root.empty()) {
		for (uint i = 0; i < root.size(); i++)
			root[i]->Cleanup();
	}
	root.clear();

	c_mesh = nullptr;

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
		


	
	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	if (!root.empty()) {
		for (uint i = 0; i < root.size(); i++)
			root[i]->Update();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

void ModuleSceneIntro::CreateGameObject(const std::string & name, const std::string & Filename)
{


	int nMeshes = App->importer->HowManyMeshes(Filename);
	const std::string helper("test");

	for (uint i = 0; i < nMeshes; i++) {

		GameObject* GO = nullptr;

		if (name.empty())
			GO = new GameObject(helper, Filename, i);
		else
		{
			char helper_2[32];
			sprintf(helper_2, "_%d", i);
			std::string fin = name + helper_2;
			GO = new GameObject(fin, Filename, i);
		}
		root.push_back(GO);
	}
}

void ModuleSceneIntro::CreateRootGameObject(const std::string & name, const std::string & filename)
{
	GameObject* Go = nullptr;



}

void ModuleSceneIntro::CreateGameObjectPS(const std::string & name, PrimitiveTypes p_shape)
{
	const std::string helper("test");
	GameObject* GP = nullptr;

	if (name.empty())
		GP = new GameObject(helper, p_shape);
	else
		GP = new GameObject(name, p_shape);

	root.push_back(GP);
}

void ModuleSceneIntro::DeleteGameObject(int to_delete)
{
	root[to_delete]->Cleanup();
	root.erase(root.begin() + to_delete);
	selected = -1;
}

void ModuleSceneIntro::MakeChecker()
{
	int i, j, c;

	for (i = 0; i < imageheight; i++) {
		for (j = 0; j < imagewidht; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}

