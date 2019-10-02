#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <string>


#define MAX_SNAKE 8

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

public:
	
	bool show_demo_window = false;

	uint my_id = 0;
	int vertexs[9] = { 
		0, 0, 0, 
		2, 0, 0,
		0, 2, 0 };
};
