#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "Json/json.hpp"
#include "GameObject.h"
#include "ModuleImporter.h"
#include "ModuleFileSystem.h"
#include "QuadTree.h"
#include "ResourceMesh.h"
#include "btPrimitive.h"
#include "PhysBody.h"
#include "ModulePhysics.h"
#include "Vehicle.h"
#include "MathGeoLib/include/Algorithm/Random/LCG.h"
#include "MathGeoLib/include/Geometry/AABB.h"
#include "MathGeoLib/include/Math/float3.h"

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

	
	quadtree = new QT(AABB(float3(-100, -10, -100), float3(100, 10, 100)), 4);
	//game_t.Start();

	
	//cam_sphere = aux_sphere;
	//cam_sphere.SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
	//cam_sphere.my_body = App->physics->AddBody(cam_sphere);

	CameraCreating();

	return ret;
}

void ModuleSceneIntro::CameraCreating()
{
	btSphere aux_sphere(1.0f);
	cam_sphere = App->physics->AddBody(aux_sphere, 1);


	cam_sphere->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);
}

update_status ModuleSceneIntro::PreUpdate(float dt)
{
	if (!house_loaded)
	{
		if (second_cycle)
		{
			App->importer->aiParentNode("Assets/Street environment_V01.fbx");
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
				if (selected->b_mesh)
				{
					selected->my_mesh->RecalcBoundingBox();
					selected->transform->bbox_changed = false;
				}
				
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

	if (quadtree)
		quadtree->CleanUp();


	c_mesh = nullptr;

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		btSphere s(10.0f);
		float force = 50.0f;
		s.SetPos(App->camera->Position.x + (App->camera->main_cam->frustum.front.x*3), App->camera->Position.y + (App->camera->main_cam->frustum.front.y * 3), App->camera->Position.z + (App->camera->main_cam->frustum.front.z * 3));//App->camera->Position.x, App->camera->Position.y, App->camera->Position.z
		s.my_body = App->physics->AddBody(s, 1);
		s.my_body->Push(App->camera->main_cam->frustum.front.x * force, App->camera->main_cam->frustum.front.y * force, App->camera->main_cam->frustum.front.z * force);
		list_spheres.push_back(s);
	}
	cam_sphere->body->activate(true);
	cam_sphere->body->setGravity({0,0.00001,0});
	cam_sphere->SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{
	RenderCubes();
	RenderSpheres();
	RenderCylinders();

	if (last_time_go != root.size())
	{
		re_quadtree = true;
	}
	if (dr_quadtree)
		quadtree->Draw();

	if (re_quadtree)
	{
		quadtree->CleanUp();
		quadtree->Create(AABB(float3(-100, -10, -100), float3(100, 10, 100)), 4);
		// then we charge all the _statics objects.
		std::vector<GameObject*> frustum_load;
		for (uint i = 0; i < root.size(); i++)
		{
			root[i]->FrustrumQuad(frustum_load);
		}
		for (uint i = 0; i < frustum_load.size(); i++)
		{
			if (!frustum_load[i]->_static)
			{
				frustum_load.erase(frustum_load.begin() + i);
				i = -1; // to reset the for.
			}
		}
		quadtree->Fill(frustum_load);
		re_quadtree = false;
	}
	last_time_go = root.size();


	FrustrumQuad();


	if (!root.empty()) {
		for (uint i = 0; i < root.size(); i++)
			root[i]->Update();
	}

	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* body1, PhysBody* body2)
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
	selected->Cleanup();
	for (uint i = 0; i < root.size(); i++)
	{
		if (selected == root[i])
		{
			root.erase(root.begin() + i);
		}
	}

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


	nlohmann::json download;
	std::ifstream k(path.c_str());
	if (!k) {
		LOG("Could not open config_file");
	}
	else {
		LOG("Config_file succesfully loaded");
		k >> download;
	}

	/// Test:
	/// Project vs 1:

	// usefull vars: 
	int num_gameobjects = 0; // Here we will store the number of gameobjects that we wanna load
	int iterator = 0; //when we run around the document we wanna know the actual iterator of the root we are working on.

	// First we want to know how many gameobjects do we have.
	for (nlohmann::json::iterator it = download.begin(); it != download.end(); it++)
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
	for (nlohmann::json::iterator it = download.begin(); it != download.end(); it++)
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



	for (int i = 0; i < root.size(); i++)
	{
		root[i]->transform->NewTransform();
	}

	
	

	//and we did a new load of scene :D I wanna dieeeeee


}

void ModuleSceneIntro::FrustrumQuad()
{
	std::vector<GameObject*> helper;

	quadtree->Intersect(helper, App->camera->main_cam->frustum);

	for (uint i = 0; i < helper.size(); i++)
	{
		if (helper[i]->b_mesh)
		{
			if (helper[i]->cam->DoCulling(helper[i]))
				helper[i]->my_mesh->inside_frustum = true;
		}
	}



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

bool ModuleSceneIntro::RayTestAABB(LineSegment ray)
{
	std::vector<GameObject*> intersected_go;

	for (std::vector<GameObject*>::iterator it = root.begin(); it != root.end(); it++)
		(*it)->FrustrumQuad(intersected_go);


	std::vector<GameObject*> intersected_go2; //We create this list to use it later to check for the triangles of the intersected GameObjects
	for (std::vector<GameObject*>::iterator it1 = intersected_go.begin(); it1 != intersected_go.end(); it1++)
	{
		if (ray.Intersects((*it1)->CreateOBB()))
		{
			LOG("COLLISIOOON");
			intersected_go2.push_back(*it1);
			RayTestTriangles(ray, intersected_go2);
		}
	}

	return true;
}

bool ModuleSceneIntro::RayTestTriangles(LineSegment last_ray, std::vector<GameObject*> intersected)
{
	float far_hit_distance = App->camera->main_cam->frustum.farPlaneDistance; // the furthest distance the ray can have
	LOG("FIRST FAR DIST: %f", far_hit_distance);
	for (std::vector<GameObject*>::iterator it = intersected.begin(); it != intersected.end(); it++)
	{
		if ((*it)->b_mesh)
		{




			if ((*it)->my_mesh->my_reference->my_mesh->num_index > 7)
			{
				for (uint i = 0; i < (*it)->my_mesh->my_reference->my_mesh->num_index; i += 3)
				{

					LineSegment local_ray = last_ray;
					local_ray.Transform((*it)->transform->GetGlobalMatrix().Inverted()); //We make the transform of the ray to be local for the triangles

					uint c_i = (*it)->my_mesh->my_reference->my_mesh->index[i] * 3;
					float3 a((*it)->my_mesh->my_reference->my_mesh->vertex[c_i], (*it)->my_mesh->my_reference->my_mesh->vertex[c_i + 1], (*it)->my_mesh->my_reference->my_mesh->vertex[c_i + 2]);
					c_i = (*it)->my_mesh->my_reference->my_mesh->index[i + 1] * 3;
					float3 b((*it)->my_mesh->my_reference->my_mesh->vertex[c_i], (*it)->my_mesh->my_reference->my_mesh->vertex[c_i + 1], (*it)->my_mesh->my_reference->my_mesh->vertex[c_i + 2]);
					c_i = (*it)->my_mesh->my_reference->my_mesh->index[i + 2] * 3;
					float3 c((*it)->my_mesh->my_reference->my_mesh->vertex[c_i], (*it)->my_mesh->my_reference->my_mesh->vertex[c_i + 1], (*it)->my_mesh->my_reference->my_mesh->vertex[c_i + 2]);
					Triangle tri(a, b, c); //We build the triangles
				//	LOG("SECOND FAR DISTANCE: %f", far_hit_distance)
						float hit_distance = 0.0f; //As it says, the distance of the hit
					if (local_ray.Intersects(tri, &hit_distance, nullptr)) //if the local ray intersects with a triangle we also get the hit distance
					{
					//	LOG("HIT POINT DISTANCE: %f", hit_distance);
						if (hit_distance < far_hit_distance) {
							far_hit_distance = hit_distance; //We get the closest distance to the hit point so we get the closest GameObject
							selected = (*it);
						}
					}
				}
			}

		}
	}
	return true;
}

void ModuleSceneIntro::CreateCube(float3 dim, float3 pos, Color color, float angle, float3 u, float mass)
{
	btCube cub(dim.x, dim.y, dim.z);
	cub.SetPos(pos.x, pos.y, pos.z);
	cub.color = color;

	if (angle != 0)
		cub.SetRotation(angle, float3(u.x, u.y, u.z));


	App->physics->AddBody(cub, mass);
	list_cubes.push_back(cub);
}

void ModuleSceneIntro::RenderCubes()
{
	std::list<btCube>::iterator cubes_item = list_cubes.begin();

	while (cubes_item != list_cubes.end()) {
		(cubes_item)->my_body->GetTransform(*(cubes_item)->transform.v);
		(cubes_item)->Render();

		++cubes_item;
	}
}

void ModuleSceneIntro::CreateSphere(float radius, float3 pos, float mass)
{
	btSphere sph(radius);
	sph.SetPos(pos.x, pos.y, pos.z);

	App->physics->AddBody(sph, mass);
	list_spheres.push_back(sph);
}

void ModuleSceneIntro::RenderSpheres()
{
	std::list<btSphere>::iterator spheres_item = list_spheres.begin();

	while (spheres_item != list_spheres.end()) {
		(spheres_item)->my_body->GetTransform(*(spheres_item)->transform.v);
		(spheres_item)->Render();
		++spheres_item;
	}
}

void ModuleSceneIntro::CreateCylinder(float radius, float height, float3 pos, float mass)
{
	btCylinder cylinder(radius, height);
	cylinder.SetPos(pos.x, pos.y, pos.z);

	App->physics->AddBody(cylinder, mass);
	list_cylinders.push_back(cylinder);
}

void ModuleSceneIntro::RenderCylinders()
{
	std::list<btCylinder>::iterator cylinders_item = list_cylinders.begin();

	while (cylinders_item != list_cylinders.end()) {
		(cylinders_item)->my_body->GetTransform(*(cylinders_item)->transform.v);
		(cylinders_item)->Render();
		++cylinders_item;
	}
}

void ModuleSceneIntro::CleanLists()
{
	list_cubes.clear();
	list_spheres.clear();
	list_cylinders.clear();
}

