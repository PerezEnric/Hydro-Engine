#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Glew/include/glew.h"

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
	uint my_tex = 0;
	uint my_id = 0;
	uint my_indices = 0;
	float vertices[108] = { 
		0.f, 0.f, 0.f, //FRONT
		2.f, 0.f, 0.f,
		0.f, 2.f, 0.f,

		2.f, 0.f, 0.f,
		2.f, 2.f, 0.f,
		0.f, 2.f, 0.f,

		2.f, 0.f, 0.f, //RIGHT
		2.f, 0.f, -2.f,
		2.f, 2.f, 0.f,

		2.f, 0.f, -2.f,
		2.f, 2.f, -2.f,
		2.f, 2.f, 0.f,

		2.f, 0.f, -2.f, //BACK
		0.f, 0.f, -2.f,
		2.f, 2.f, -2.f,

		0.f, 0.f, -2.f,
		0.f, 2.f, -2.f,
		2.f, 2.f, -2.f,

		0.f, 0.f, -2.f, //LEFT
		0.f, 0.f, 0.f,
		0.f, 2.f, -2.f,

		0.f, 0.f, 0.f,
		0.f, 2.f, 0.f,
		0.f, 2.f, -2.f,

		0.f, 2.f, 0.f, //TOP
		2.f, 2.f, 0.f,
		0.f, 2.f, -2.f,

		2.f, 2.f, 0.f,
		2.f, 2.f, -2.f,
		0.f, 2.f, -2.f,

		0.f, 0.f, -2.f, //BOTTOM
		2.f, 0.f, -2.f,
		0.f, 0.f, 0.f,

		2.f, 0.f, -2.f,
		2.f, 0.f, 0.f,
		0.f, 0.f, 0.f,
		};

	int indices[36] =
	{
		0, 1, 2,  1, 3, 2, //FRONT

		1, 4, 3,  4, 5, 3, //RIGHT

		4, 6, 5,  6, 7, 5, //BACK

		6, 0, 7,  0, 2, 7, //LEFT

		2, 3, 7,  3, 5, 7, //TOP

		6, 4, 0,  4, 1, 0, //BOTTOM

	};

	float vertices2[24] = {
		0.f, 0.f, 0.f, //A->0
		2.f, 0.f, 0.f, //B->1
		0.f, 2.f, 0.f, //C->2
		2.f, 2.f, 0.f, //D->3
		2.f, 0.f, -2.f, //E->4
		2.f, 2.f, -2.f, //F->5
		0.f, 0.f, -2.f, //G->6
		0.f, 2.f, -2.f, //H->7
	};

	//float textures[16] = {
	//	0.0f, 0.0f,
	//	1.0f, 0.0f,
	//	0.0f, 1.0f,
	//	1.0f, 1.0f,
	//	1.0f, 0.0f,
	//	1.0f, 1.0f,
	//	0.0f, 1.0f,
	//	0.0f, 0.0f,
	//};

	void MakeChecker();

	uint imagewidht = 64;
	uint imageheight = 64;
	GLubyte checkImage[64][64][4];

	GLuint texName;

};
