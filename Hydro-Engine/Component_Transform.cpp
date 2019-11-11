#include "Application.h"
#include "GameObject.h"
#include "Component_Transform.h"
#include "ModuleSceneIntro.h"
#include "ImGui/imgui.h"

Component_Transform::Component_Transform(GameObject* GO, COMPONENT_TYPE type): Component(GO, type)
{

}

Component_Transform::Component_Transform()
{
}

float3 Component_Transform::GetPosition()
{
	return float3(App->scene_intro->selected->GO_position.x,
		App->scene_intro->selected->GO_position.y,
		App->scene_intro->selected->GO_position.z);
}

float3 Component_Transform::GetRotation()
{
	return float3(App->scene_intro->selected->GO_rotation.x,
		App->scene_intro->selected->GO_rotation.y,
		App->scene_intro->selected->GO_rotation.z);
}

float3 Component_Transform::GetScale()
{
	return float3(App->scene_intro->selected->GO_scale.x,
		App->scene_intro->selected->GO_scale.y,
		App->scene_intro->selected->GO_scale.z);
}

void Component_Transform::SetPosition(float3 position)
{
	//App->scene_intro->root[App->scene_intro->selected]->GO_position = position;

	for (uint i = 0; i < (App->scene_intro->selected->my_mesh.num_vertex * 3); i += 3)
	{
		App->scene_intro->selected->my_mesh.vertex[i] += position.x;
		App->scene_intro->selected->my_mesh.vertex[i + 1] += position.y;
		App->scene_intro->selected->my_mesh.vertex[i + 2] += position.z;
	}


	LOG("%f", position.x);
}

Component_Transform * Component_Transform::GetThis()
{
	return this;
}

void Component_Transform::ShowInfo()
{
	//ImGui::Text("Position: %.2f %.2f %.2f", App->scene_intro->root[App->scene_intro->selected]->GO_position.x, 
	//	App->scene_intro->root[App->scene_intro->selected]->GO_position.y, 
	//	App->scene_intro->root[App->scene_intro->selected]->GO_position.z);

	//ImGui::Text("Rotation: %.2f %.2f %.2f", App->scene_intro->root[App->scene_intro->selected]->GO_rotation.x, 
	//	App->scene_intro->root[App->scene_intro->selected]->GO_rotation.y, 
	//	App->scene_intro->root[App->scene_intro->selected]->GO_rotation.z);

	//ImGui::Text("Scale: %.2f %.2f %.2f", 
	//	App->scene_intro->root[App->scene_intro->selected]->GO_scale.x, 
	//	App->scene_intro->root[App->scene_intro->selected]->GO_scale.y, 
	//	App->scene_intro->root[App->scene_intro->selected]->GO_scale.z);

	//ImGui::Text("Position: %.2f %.2f %.2f", GO_position.x,
	//	GO_position.y,
	//	GO_position.z);

	//ImGui::Text("Rotation: %.2f %.2f %.2f", GO_rotation.x,
	//	GO_rotation.y,
	//	GO_rotation.z);

	//ImGui::Text("Scale: %.2f %.2f %.2f",
	//	GO_scale.x,
	//	GO_scale.y,
	//	GO_scale.z);
}
