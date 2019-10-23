#include "GameObject.h"
#include "Component_Mesh.h"
#include "Component_Texture.h"
#include "ModuleImporter.h"
#include "Application.h"



GameObject::GameObject(const std::string & name)
{
	this->name = name;

}

GameObject::GameObject(const std::string & name, const std::string & Filename, int index)
{
	// Mesh CX.
	this->name = name;
	this->path = Filename;
	this->actual_mesh = index;
	/*mesh_array = App->importer->HowManyMeshes(Filename);*/

	//for (int i = 0; i < mesh_array; i++)
	CreateComponent(MESH);
}

GameObject::GameObject(const std::string & name, PrimitiveTypes type)
{
	this->name = name;
	this->p_type = type;

	CreateComponent(MESH);

}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->active)
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
	case TEXTURE:
		my_comp = new Component_Texture(this, type);
		break;
	}
	components.push_back(my_comp);

	return my_comp;
}

void GameObject::Cleanup()
{
}
