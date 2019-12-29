#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "btPrimitive.h"
#include "GameTimer.h"
#include "Glew/include/glew.h"
#include "Json/json.hpp"
#include <queue>
#include <string>



#define MAX_SNAKE 8
class GameObject;
class btCube;
class btSphere;
struct PhysBody;
class QT;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	void CameraCreating();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnCollision(PhysBody* body1, PhysBody* body2);

	void CreateGameObject(const std::string & name, const std::string & Filename);

	void CreateRootGameObject(const std::string & name, const std::string & Filename);

	void CreateGameObjectPS(const std::string & name, PrimitiveTypes p_shape);

	void DeleteGameObject();

	void ChangeJson(nlohmann::json & to_change);

	void LoadScene(std::string path); 

	void FrustrumQuad();

	//this funct creates an empty gameobject.
	void CreateEmptyGameObject();

	void SaveScene(std::string path); 

	bool RayTestAABB(LineSegment ray);
	bool RayTestTriangles(LineSegment last_ray, std::vector<GameObject*> intersected);

	void CreateCube(float3 dimension = { 10.0f, 10.0f, 10.0f }, float3 position = { 0.0f, 0.0f, 0.0f }, Color color = White, float angle = 0.0f, float3 u = { 0.0f, 0.0f, 0.0f }, float mass = 0.0f);
	void RenderCubes();

	void CreateSphere(float radius = 1.0f, float3 pos = { 0.0f, 0.0f, 0.0f }, float mass = 0.0f);
	void RenderSpheres();

	void CreateCylinder(float radius = 1.0f, float height = 1.0f, float3 pos = { 0.0f, 0.0f, 0.0f }, float mass = 0.0f);
	void RenderCylinders();

public:
	// Game Objects.
	std::vector<GameObject*> root;
	GameObject* selected = nullptr;
	GameObject* aux = nullptr;

	bool show_demo_window = false;

	uint my_tex = 0;
	uint my_id = 0;
	uint my_indices = 0;

	Primitive c;
	std::list<btCube> list_cubes;
	std::list<btSphere> list_spheres;
	std::list<btCylinder> list_cylinders;
	PhysBody* cam_sphere;
	void MakeChecker();

	uint imagewidht = 64;
	uint imageheight = 64;
	GLubyte checkImage[64][64][4];

	GLuint texName;

	par_shapes_mesh* c_mesh;

	bool house_loaded = false;
	bool second_cycle = false;


	//QuadTree

	QT* quadtree = nullptr;
	int last_time_go = 0;
	bool re_quadtree = false;
	bool dr_quadtree = false;

	// Timer

	GameTimer game_t;
};
