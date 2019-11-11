#include "GameObject.h"
#include "Component_Mesh.h"
#include "Component_Texture.h"
#include "Component_Transform.h"
#include "ModuleImporter.h"
#include "Application.h"
#include "ImGui/imgui.h""



GameObject::GameObject(const std::string & name)
{
	this->name = name;
	CreateComponent(TRANSFORM);
}

GameObject::GameObject(const std::string & name, const std::string & Filename, int index)
{
	// Mesh CX.
	this->name = name;
	this->path = Filename;
	this->actual_mesh = index;
	CreateComponent(TRANSFORM);
	CreateComponent(MESH);
}

GameObject::GameObject(const std::string & name, PrimitiveTypes type)
{
	this->name = name;
	this->p_type = type;

	CreateComponent(MESH);
	CreateComponent(TRANSFORM);
}

GameObject::GameObject(const std::string & name, const std::string & Filename)
{
	this->name = name;
	this->path = Filename;
	
	App->importer->CreateGO(Filename, this);
	
	CreateComponent(TRANSFORM);

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

	if (App->scene_intro->selected == nullptr)
		LOG("I wanna destroy the world");
}

Component * GameObject::CreateComponent(COMPONENT_TYPE type)
{
	Component* my_comp = nullptr;

	switch (type)
	{
	case MESH:
		my_comp = new Component_Mesh(this, type);
		break;
	case TEXTURE:
		if (!texture)
			my_comp = new Component_Texture(this, type);
		break;
	case TRANSFORM:
		my_comp = new Component_Transform(this, type);
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

	name.clear();
	
}

void GameObject::CreateChildren(const std::string & name, const std::string & Filename, int index)
{
	GameObject* Go = nullptr;

	Go = new GameObject(name, Filename, index);

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

AABB GameObject::CreateBBox()
{
	AABB bbox;

	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->type == MESH)
		{
			bbox = components[i]->CreateBBox();
		}
	}
	return bbox;
}
