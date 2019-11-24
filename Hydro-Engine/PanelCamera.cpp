#include "Application.h"
#include "PanelCamera.h"
#include "ModuleCamera3D.h"
#include "Component_Camera.h"
#include "ImGui/imgui.h"

PanelCamera::PanelCamera()
{
}

PanelCamera::~PanelCamera()
{
}

bool PanelCamera::Update()
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Camera Settings", &is_active, flags);

	ImGui::SetWindowPos({ 200, 120.0f });
	ImGui::SetWindowSize(ImVec2(250, 100));

	if (ImGui::DragFloat("Near Plane", &App->camera->main_cam->frustum.nearPlaneDistance, 0.1f, 0.1f, App->camera->main_cam->frustum.farPlaneDistance))
	{
		App->camera->main_cam->ChangeNearPlaneDist(App->camera->main_cam->frustum.nearPlaneDistance);
	}

	if (ImGui::DragFloat("Far Plane", &App->camera->main_cam->frustum.farPlaneDistance, 0.1f, App->camera->main_cam->frustum.nearPlaneDistance, 500.0f))
	{
		App->camera->main_cam->ChangeFarPlaneDist(App->camera->main_cam->frustum.farPlaneDistance);
	}

	if (ImGui::DragFloat("FOV", &App->camera->main_cam->angle_fov, 0.1f))
	{
		App->camera->main_cam->ChangeFOV(App->camera->main_cam->angle_fov);
	}

	ImGui::End();
	
	return true;
}
