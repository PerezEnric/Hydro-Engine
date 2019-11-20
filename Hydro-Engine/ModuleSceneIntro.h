#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Glew/include/glew.h"
#include "Json/json.hpp"
#include <queue>
#include <string>



#define MAX_SNAKE 8
class GameObject;
struct PhysBody3D;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreateGameObject(const std::string & name, const std::string & Filename);

	void CreateRootGameObject(const std::string & name, const std::string & Filename);

	void CreateGameObjectPS(const std::string & name, PrimitiveTypes p_shape);

	void DeleteGameObject();

	void ChangeJson(nlohmann::json & to_change);

	void LoadScene(); //ghoy debes cambiar esto luego para que digan que scene quieren loguear

	//this funct creates an empty gameobject.
	void CreateEmptyGameObject();

	/*void SaveScene();*/ //ghoy debes cambiar esto luego para que digan que nombre le ponen a la escena.

	nlohmann::json cancer();



public:
	// Game Objects.
	std::vector<GameObject*> root;
	GameObject* selected = nullptr;
	

	bool show_demo_window = false;

	uint my_tex = 0;
	uint my_id = 0;
	uint my_indices = 0;

	Primitive c;

	void MakeChecker();

	uint imagewidht = 64;
	uint imageheight = 64;
	GLubyte checkImage[64][64][4];

	GLuint texName;

	par_shapes_mesh* c_mesh;

	bool house_loaded = false;
	bool second_cycle = false;
};
