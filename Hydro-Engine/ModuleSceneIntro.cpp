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

	
	//MakeChecker();

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glGenTextures(1, &texName);
	//glBindTexture(GL_TEXTURE_2D, texName);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imagewidht, imageheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);



	return ret;
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	//LOG("Unloading Intro scene");
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
	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
		CreateGameObject("house", "Assets/BakerHouse.fbx");


	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		CreateGameObject("tet", "Assets/warrior.fbx");

	
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, texName);

	//Plane p(0, 1, 0, 0);
	//p.axis = true;
	//p.Render();
	

	//glBegin(GL_TRIANGLES);


	////FRONT
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 0.f, 0.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 2.f, 0.f);

	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 0.f, 0.f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(2.f, 2.f, 0.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 2.f, 0.f);

	////RIGHT

	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(2.f, 0.f, 0.f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 0.f, -2.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(2.f, 2.f, 0.f);

	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 0.f, -2.f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(2.f, 2.f, -2.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(2.f, 2.f, 0.f);

	////BACK

	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(2.f, 0.f, -2.f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(0.f, 0.f, -2.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(2.f, 2.f, -2.f);

	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(0.f, 0.f, -2.f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(0.f, 2.f, -2.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(2.f, 2.f, -2.f);

	////LEFT

	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(0.f, 0.f, -2.f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 2.f, -2.f);

	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(0.f, 0.f, 0.f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(0.f, 2.f, 0.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 2.f, -2.f);

	////TOP
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(0.f, 2.f, 0.f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 2.f, 0.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 2.f, -2.f);

	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 2.f, 0.f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(2.f, 2.f, -2.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 2.f, -2.f);

	////BOTTOM
	//glTexCoord2f(0.0f, 0.0f);
	//glVertex3f(0.f, 0.f, -2.f);
	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 0.f, -2.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 0.f, 0.f);

	//glTexCoord2f(1.0f, 0.0f);
	//glVertex3f(2.f, 0.f, -2.f);
	//glTexCoord2f(1.0f, 1.0f);
	//glVertex3f(2.f, 0.f, 0.f);
	//glTexCoord2f(0.0f, 1.0f);
	//glVertex3f(0.f, 0.f, 0.f);

	//glEnd();
	//

	//glGenBuffers(1, (GLuint*) & (my_id));
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*36*3, vertices, GL_STATIC_DRAW);
	//
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDisable(GL_TEXTURE_2D);

	////// deactivate vertex arrays after drawing
	//glDisableClientState(GL_VERTEX_ARRAY);
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texName);
	


	
	
	glGenBuffers(1, (GLuint*) & (my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);

	//glGenBuffers(1, (GLuint*) & (m_indices));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices, GL_STATIC_DRAW);


	glGenBuffers(1, (GLuint*) & (my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertices2, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*) & (my_tex));
	glBindBuffer(GL_ARRAY_BUFFER, my_tex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, textures, GL_STATIC_DRAW);


	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, my_tex);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glFlush();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	*/
	/*par_shapes_mesh* cube = par_shapes_create_cube();
	par_shapes_translate(cube, 1, 0, 0.5);
	par_shapes_free_mesh(cube);*/

	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	//glDisableClientState(GL_VERTEX_ARRAY);

	



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
			GO = new GameObject(name, Filename, i);

		root.push_back(GO);
	}
	


	
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

