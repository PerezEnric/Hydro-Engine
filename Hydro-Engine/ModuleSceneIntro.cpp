#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Json/json.hpp"
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
	name = j["Config"]["App"]["Name"].get<std::string>();
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
	//Plane p(0, 1, 0, 0);
	//p.axis = true;
	//p.Render();
	
	//glBegin(GL_TRIANGLES);

	////FRONT
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(2.f, 0.f, 0.f);
	//glVertex3f(0.f, 2.f, 0.f);

	//glVertex3f(2.f, 0.f, 0.f);
	//glVertex3f(2.f, 2.f, 0.f);
	//glVertex3f(0.f, 2.f, 0.f);

	////RIGHT

	//glVertex3f(2.f, 0.f, 0.f);
	//glVertex3f(2.f, 0.f, -2.f);
	//glVertex3f(2.f, 2.f, 0.f);

	//glVertex3f(2.f, 0.f, -2.f);
	//glVertex3f(2.f, 2.f, -2.f);
	//glVertex3f(2.f, 2.f, 0.f);

	////BACK

	//glVertex3f(2.f, 0.f, -2.f);
	//glVertex3f(0.f, 0.f, -2.f);
	//glVertex3f(2.f, 2.f, -2.f);

	//glVertex3f(0.f, 0.f, -2.f);
	//glVertex3f(0.f, 2.f, -2.f);
	//glVertex3f(2.f, 2.f, -2.f);

	////LEFT

	//glVertex3f(0.f, 0.f, -2.f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 2.f, -2.f);

	//glVertex3f(0.f, 0.f, 0.f);
	//glVertex3f(0.f, 2.f, 0.f);
	//glVertex3f(0.f, 2.f, -2.f);

	////TOP
	//glVertex3f(0.f, 2.f, 0.f);
	//glVertex3f(2.f, 2.f, 0.f);
	//glVertex3f(0.f, 2.f, -2.f);

	//glVertex3f(2.f, 2.f, 0.f);
	//glVertex3f(2.f, 2.f, -2.f);
	//glVertex3f(0.f, 2.f, -2.f);

	////BOTTOM

	//glVertex3f(0.f, 0.f, -2.f);
	//glVertex3f(2.f, 0.f, -2.f);
	//glVertex3f(0.f, 0.f, 0.f);

	//glVertex3f(2.f, 0.f, -2.f);
	//glVertex3f(2.f, 0.f, 0.f);
	//glVertex3f(0.f, 0.f, 0.f);

	//glEnd();

	//glGenBuffers(1, (GLuint*) & (my_id));
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*36*3, vertices, GL_STATIC_DRAW);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//// deactivate vertex arrays after drawing
	//glDisableClientState(GL_VERTEX_ARRAY);

	//glGenBuffers(1, (GLuint*) & (m_indices));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, vertices2);

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	//glDisableClientState(GL_VERTEX_ARRAY);
	float a = 0.0f;
	c.CreatePrimitive(PrimitiveTypes::P_CUBE, &a);


	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

