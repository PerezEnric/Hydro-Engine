#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Json/json.hpp"
#include <fstream>
#include <istream>
#include <string>

//#include "MathGeoLib/include/MathGeoLib.h"
//#include "MathGeoLib/include/MathBuildConfig.h"
//#include "MathGeoLib/include/MathGeoLibFwd.h"




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

	/*App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	/*App->camera->LookAt(vec3(0, 0, 0));*/
	/*math::Sphere s1({ 0, 0, 0 }, 2);
	math::Sphere s2({ 1, 1, 0 }, 2);

	bool is_inter = false;

	s1.Intersects(s2);

	

	math::LCG rn = math::LCG();
	int test;
	test = rn.Int(5, 15);

	if (s1.Intersects(s2))
	{
		is_inter = true;
	}*/
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
	name = j["App"]["Name"].get<std::string>();
	App->window->SetTitle(name.c_str());
	

	return ret;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");


	return true;
}



// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();
	





	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

