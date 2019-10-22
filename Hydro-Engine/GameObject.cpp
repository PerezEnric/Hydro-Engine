#include "GameObject.h"
#include "Component_Mesh.h"
#include "ModuleImporter.h"
#include "Application.h"



GameObject::GameObject(const std::string & name)
{
	this->name = name;

}

GameObject::GameObject(const std::string & name, const std::string & Filename)
{
	// Mesh CX.
	this->name = name;
	this->path = Filename;
	mesh_array = App->importer->HowManyMeshes(Filename);


	for (int i = 0; i < mesh_array; i++)
		components.push_back(CreateComponent(MESH));
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (uint i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

Component * GameObject::CreateComponent(COMPONENT_TYPE type)
{
	Component* my_comp;

	switch (type)
	{
	case MESH:
		my_comp = new Component_Mesh(this, type);
		break;
	}

	return my_comp;
}
