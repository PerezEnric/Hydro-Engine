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

	if (selected != nullptr)
	{
		if (selected->b_transform)
		{
			if (selected->transform->bbox_changed)
			{
				selected->my_mesh->RecalcBoundingBox();
				selected->transform->bbox_changed = false;
			}
		}
			
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

void ModuleSceneIntro::LoadScene(std::string path)
{
	//first we wanna delete our current scene cleaning all the gameobjects.

	if (!root.empty()) {
		for (uint i = 0; i < root.size(); i++)
			root[i]->Cleanup();
	}
	root.clear();


	nlohmann::json descarga;
	std::ifstream k("Assets/Scene.json");
	if (!k) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		k >> descarga;
	}

	/// Test:
	//for (nlohmann::json::iterator it = descarga.begin(); it != descarga.end(); it++)
	//{
	//	nlohmann::json hola = it.value();
	//	int que = 30;
	//	que = hola["actual mesh"].get<int>();
	//	if (hola.is_object())
	//	{
	//		LOG("%d", que);
	//	}
	//	// Esto funciona asi que tira por aqui gollim del futuro :D
	//}
	/// Project vs 1:

	// usefull vars: 
	int num_gameobjects = 0; // Here we will store the number of gameobjects that we wanna load
	int iterator = 0; //when we run around the document we wanna know the actual iterator of the root we are working on.

	// First we want to know how many gameobjects do we have.
	for (nlohmann::json::iterator it = descarga.begin(); it != descarga.end(); it++)
	{
		num_gameobjects++;
	}

	LOG("%i", num_gameobjects);

	// Then we want to create empty carcases and push it to root by now. (in the future we will make them have the relation of father children)
	for (int i = 0; i < num_gameobjects; i++)
	{
		CreateEmptyGameObject();
	}

	//Then we want to send them their info to complet their data.
	for (nlohmann::json::iterator it = descarga.begin(); it != descarga.end(); it++)
	{
		nlohmann::json GamObj = it.value();
		root[iterator]->LoadGameObject(GamObj);
		iterator++;
	}

	//when we finished creating our gameobjects we wanna bond them father - child. So...
	for (int i = 0; i < root.size(); i++)
	{
		for (int j = 0; j < root.size(); j++)
		{
			if (root[j]->my_uuid == root[i]->parent_uuid)
			{
				root[i]->parent = root[j];
				root[j]->childrens.push_back(root[i]); // double conection.
				break;
			}
		}
	}
	
	//once we finished we wanna delete the pointer in root.
	for (int i = 0; i < root.size(); i++)
	{
		if (root[i]->parent_uuid == 0)
		{ }
		else
		{
			root.erase(root.begin() + i);
			i = -1;
		}	
	}

	//and we did a new load of scene :D I wanna dieeeeee


}

void ModuleSceneIntro::CreateEmptyGameObject()
{
	GameObject* empty = nullptr;

	empty = new GameObject();

	root.push_back(empty);
}

void ModuleSceneIntro::SaveScene(std::string path)
{
	nlohmann::json save;

	for (int i = 0; i < root.size(); i++)
		root[i]->SaveGameObject(save);
	
	//App->file_system->SaveFile(path.c_str(), save);


	std::string output;
	std::string buffer;
	buffer = save.dump();
	App->file_system->SaveUnique(output, buffer.c_str(), buffer.size()*sizeof(char), LIBRARY_SCENE_FOLDER, path.c_str(), "json");


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

