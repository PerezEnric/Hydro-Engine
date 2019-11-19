#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Json/json.hpp"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "ModuleFileSystem.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
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


	
	nlohmann::json descarga;
	std::ifstream k("Assets/Scene.json");
	if (!k) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		k >> descarga;
	}

	for (nlohmann::json::iterator it = descarga.begin(); it != descarga.end(); it++)
	{
		nlohmann::json hola = it.value();
		int que = 30;
		que = hola["actual mesh"].get<int>();
		if (hola.is_object())
		{
			LOG("%d", que);
		}
		// Esto funciona asi que tira por aqui gollim del futuro :D


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
			App->importer->aiParentNode("Assets/BakerHouse.fbx");
			house_loaded = true;
		}
		second_cycle = true;
	}

	if (selected != nullptr && selected->transform->bbox_changed)
	{
		selected->my_mesh->RecalcBoundingBox();
		selected->transform->bbox_changed = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		nlohmann::json t;
		for (uint i = 0; i < root.size(); i++)
		{
			root[i]->SaveGameObject(t);
		}
		App->file_system->SaveFile("Assets/Scene.json", t);
		
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
		
	//if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	//{
	//	OBB b = selected->my_meshCreateOBB();
	//}

	
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


	//int nMeshes = App->importer->HowManyMeshes(Filename);
	//const std::string helper("test");

	//for (uint i = 0; i < nMeshes; i++) {

	//	GameObject* GO = nullptr;

	//	if (name.empty())
	//		GO = new GameObject(helper, Filename, i);
	//	else
	//	{
	//		char helper_2[32];
	//		sprintf(helper_2, "_%d", i);
	//		std::string fin = name + helper_2;
	//		GO = new GameObject(fin, Filename, i);
	//	}
	//	root.push_back(GO);
	//}
}

void ModuleSceneIntro::CreateRootGameObject(const std::string & name, const std::string & filename)
{
	GameObject* Go = nullptr;

	Go = new GameObject(name, filename, true);

	root.push_back(Go);

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

void ModuleSceneIntro::DeleteGameObject()
{
	selected->Cleanup();//todo aqui me falta eliminarlo de la array del padre.
	selected = nullptr;
}

void ModuleSceneIntro::ChangeJson(nlohmann::json & to_change)
{
	

}

nlohmann::json ModuleSceneIntro::cancer()
{
	nlohmann::json ret;

	ret["1"] = 1;
	ret["2"] = 2;


	return ret;
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

