#include "GameObject.h"
#include "Component_Mesh.h"
#include "Component_Texture.h"
#include "Component_Transform.h"
#include "Component_Camera.h"
#include "ModuleImporter.h"
#include "Application.h"
#include "ImGui/imgui.h""
#include "PCGRandom/pcg_basic.h"



GameObject::GameObject(const std::string & name)
{
	my_uuid = pcg32_random();
	this->name = name;
	CreateComponent(TRANSFORM);
	CreateComponent(CAMERA);
}

GameObject::GameObject(const std::string & name, const std::string & Filename, int index)
{
	my_uuid = pcg32_random();
	// Mesh CX.
	this->name = name;
	this->path = Filename;
	this->actual_mesh = index;
	CreateComponent(TRANSFORM);
	CreateComponent(MESH);
	CreateComponent(CAMERA);
	CreateOBB();

}

GameObject::GameObject(const std::string & name, PrimitiveTypes type)
{
	my_uuid = pcg32_random();
	this->name = name;
	this->p_type = type;
	CreateComponent(TRANSFORM);
	CreateComponent(MESH);
	CreateComponent(CAMERA);
	CreateOBB();
}

GameObject::GameObject(const std::string & name, const std::string & Filename, bool root)
{
	my_uuid = pcg32_random();
	this->name = name;
	this->path = Filename;
	CreateComponent(TRANSFORM);
	CreateComponent(CAMERA);

	if (root)
		App->importer->CreateGO(Filename, this);

	CreateOBB();
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (uint i = 0; i < components.size(); i++)
	{
		if (my_tex != nullptr && !texture)
			EliminateComponent(TEXTURE);

		if (components[i]->active)
			components[i]->Update();
	}
	for (uint i = 0; i < childrens.size(); i++)
	{
		childrens[i]->Update();
	}
}

Component * GameObject::CreateComponent(COMPONENT_TYPE type, bool _empty)
{
	Component* my_comp = nullptr;

	switch (type)
	{
	case MESH:
		my_comp = new Component_Mesh(this, type, _empty);
		break;
	case TEXTURE:
		if (!texture  || just_loading)
			my_comp = new Component_Texture(this, type, _empty);
		break;
	case TRANSFORM:
		my_comp = new Component_Transform(this, type, _empty);
		break;
	case CAMERA:
		my_comp = new Component_Camera(this, type, _empty);
		break;
	}
	if (my_comp != nullptr)
		components.push_back(my_comp);

	return my_comp;
}

void GameObject::Cleanup()
{
	for (uint i = 0; i < components.size(); i++)
	{
		components[i]->CleanUp();
	}
	components.clear();
	for (uint i = 0; i < childrens.size(); i++)
	{
		childrens[i]->Cleanup();
	}
	childrens.clear();

	if (parent != nullptr)
	{
		for (int i = 0; i < parent->childrens.size(); i++)
		{
			if (parent->childrens[i] == this) {
				parent->childrens.erase(parent->childrens.begin() + i);
				break;
			}
		}
	}
	name.clear();
}

void GameObject::CreateChildren(const std::string & name, const std::string & Filename, int index)
{
	GameObject* Go = nullptr;

	Go = new GameObject(name, Filename, index);
	Go->parent = this;
	Go->parent_uuid = my_uuid;

	childrens.push_back(Go);
}

void GameObject::EliminateComponent(COMPONENT_TYPE type)
{
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type)
		{
			components[i]->CleanUp();
			components.erase(components.begin()+i);	
		}
	}
}

void GameObject::ShowInfo(COMPONENT_TYPE type)
{
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type)
		{
			components[i]->ShowInfo();
		}
	}
}

bool GameObject::DoIhave(COMPONENT_TYPE type)
{
	bool ret = false;

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void GameObject::QuadTree(int n)
{

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (App->scene_intro->selected == this)
	{
		node_flags |= ImGuiTreeNodeFlags_Selected;
	}
		
	bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)n, node_flags, this->name.c_str(), n);
	n++;
	if (ImGui::IsItemClicked())
	{
		App->scene_intro->selected = this;
	}

	if (node_open)
	{
		for (int i = 0; i < childrens.size(); i++)
		{
			childrens[i]->QuadTree(n);
		}
		ImGui::TreePop();
	}
}

void GameObject::CreateEmptyChild(const std::string & name, const std::string & Filename)
{
	GameObject* Go = nullptr;

	Go = new GameObject(name, Filename, false);

	Go->parent = this;
	Go->parent_uuid = my_uuid;

	childrens.push_back(Go);
}

void GameObject::SaveGameObject(nlohmann::json & to_save)
{
	nlohmann::json this_GO;



	this_GO["name"] = name.c_str();
	this_GO["static"] = _static;
	this_GO["mesh array"] = mesh_array;
	this_GO["actual mesh"] = actual_mesh;
	this_GO["I have texture"] = texture;

	this_GO["Path"] = path.c_str();

	this_GO["Texture path"] = texture_path.c_str();

	// We should look at primitive types to see what we done to serialize directly this var.
	this_GO["P_type"] = p_type;
	
	//We wanna know if we have mesh tex...

	//mesh

	this_GO["b_mesh"] = b_mesh;

	//transform
	
	this_GO["b_transform"] = b_transform;

	//Camera

	this_GO["b_camera"] = b_camera;


	char* uuid_str = new char[200];

	sprintf(uuid_str, "%u", my_uuid);
	nlohmann::json this_compo;
	
	this_GO["My uuid"] = my_uuid;

	
	this_GO["My parent uuid"] = parent_uuid;
	
	
	for (uint i = 0; i < components.size(); i++)
	{
		this_compo[components[i]->comp_type_str.c_str()] = components[i]->SaveComponent();
	}
	this_GO["Components"] = this_compo;

	//Lastly we wanna make a instance in the document with 
	to_save[uuid_str] = this_GO; 
	
	// to make order.
	for (uint i = 0; i < childrens.size(); i++)
	{
		childrens[i]->SaveGameObject(to_save);
	}
}

void GameObject::LoadGameObject(nlohmann::json & to_load)
{
	// usefull variables
	std::string comp = "Components"; // with this variable we will go to the component section.
	std::string me = "mesh";
	std::string te = "texture";
	std::string tr = "transform";
	std::string ca = "camera";
	nlohmann::json g_comp;

	for (nlohmann::json::iterator it = to_load.begin(); it != to_load.end(); it++)
	{
		std::string helper = it.key();
		if (helper == comp)
		{
			g_comp = it.value(); // here we are asigning g_comp to object components in the json archive
			break;
		}	
	}

	// load ints

	this->mesh_array = to_load["mesh array"].get<int>();
	this->actual_mesh = to_load["actual mesh"].get<int>();
	this->my_uuid = to_load["My uuid"].get<uint>();
	this->parent_uuid = to_load["My parent uuid"].get<uint>();

	// load bools

	this->_static = to_load["static"].get<bool>();
	this->texture = to_load["I have texture"].get<bool>();
	this->b_camera = to_load["b_camera"].get<bool>();
	this->b_mesh = to_load["b_mesh"].get<bool>();
	this->b_transform = to_load["b_transform"].get<bool>();

	// Load Strings

	this->name = to_load["name"].get<std::string>();
	this->path = to_load["Path"].get<std::string>();
	this->texture_path = to_load["Texture path"].get<std::string>();
	this->my_uuid = to_load["My uuid"].get<int>();

	// Load Ptype

	this->p_type = to_load["P_type"].get<PrimitiveTypes>();

	


	// Load All the components that we had :D

	//First we look if the gameobject have a mesh
	if (b_mesh)
	{
		//if so we create an empty mesh.
		CreateComponent(MESH, true);
		// We search for the mesh data
		for (nlohmann::json::iterator it = g_comp.begin(); it != g_comp.end(); it++)
		{
			std::string helr = it.key();
			if (helr == me)
			{
				nlohmann::json tl = it.value();
				my_mesh->LoadComponent(tl);
				break;
			}
		}
	}


	just_loading = true;

	//texture
	if (texture)
	{
		//if so we create an empty mesh.
		CreateComponent(TEXTURE, true);
		// We search for the mesh data
		for (nlohmann::json::iterator it = g_comp.begin(); it != g_comp.end(); it++)
		{
			std::string helr = it.key();
			if (helr == te)
			{
				nlohmann::json tl = it.value();
				my_tex->LoadComponent(tl);
				break;
			}
		}
	}


	if (b_transform)
	{
		//if so we create an empty mesh.
		CreateComponent(TRANSFORM, false);
		// We search for the mesh data
		for (nlohmann::json::iterator it = g_comp.begin(); it != g_comp.end(); it++)
		{
			std::string helr = it.key();
			if (helr == tr)
			{
				nlohmann::json tl = it.value();
				transform->LoadComponent(tl);
				break;
			}
		}
	}

	if (b_camera)
	{
		//if so we create an empty mesh.
		CreateComponent(CAMERA, false);
		// We search for the mesh data
		for (nlohmann::json::iterator it = g_comp.begin(); it != g_comp.end(); it++)
		{
			std::string helr = it.key();
			if (helr == ca)
			{
				nlohmann::json tl = it.value();
				cam->LoadComponent(tl);
				break;
			}
		}
	}

	
	CreateOBB();
	


	///Esto funciona 

	//int helper = 0;
	////then we want to send to this mesh his info
	//for (nlohmann::json::iterator it = to_load.begin(); it != to_load.end(); it++)
	//{
	//	std::string helper_2 = it.key();
	//	std::string helper_3 = "Components";
	//	if (helper_2 == helper_3)
	//		helper++;
	//}
	//LOG("%i objects", helper);
}

void GameObject::FrustrumQuad(std::vector<GameObject*>& frust)
{
	frust.push_back(this);
	for (int i = 0; i < childrens.size(); i++)
		childrens[i]->FrustrumQuad(frust);
}



AABB GameObject::CreateAABB()
{
	AABB bbox;

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == MESH)
		{
			bbox = components[i]->CreateAABB();
		}
	}
	return bbox;
}

OBB GameObject::CreateOBB()
{
	OBB obbox;

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == MESH)
		{
			obbox = components[i]->CreateOBB();
		}
	}
	return obbox;
}


