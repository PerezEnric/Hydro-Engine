#include "Application.h"
#include "PanelShapes.h"
#include "ImGui/imgui.h"

PanelShapes::PanelShapes() : Panel()
{
}

PanelShapes::~PanelShapes()
{
}

bool PanelShapes::Update()
{
	bool ret = true;

	if (ImGui::Begin("Primitive shapes", &is_active))
	{
		ImGui::SetWindowPos(ImVec2{ 600, 20 }, ImGuiCond_FirstUseEver);
		ImGui::SetWindowSize(ImVec2{ 200, 100 });

		if (ImGui::Button("Cube"))
		{
			prim.CreatePrimitive(p_mesh, PrimitiveTypes::P_CUBE);
		}
		ImGui::SameLine();
		if (ImGui::Button("Sphere"))
		{
			prim.CreatePrimitive(p_mesh, PrimitiveTypes::P_SPHERE);
		}
		if (ImGui::Button("Cylinder"))
		{
			prim.CreatePrimitive(p_mesh, PrimitiveTypes::P_CYLINDER);
		}
		ImGui::SameLine();
		if (ImGui::Button("Plane"))
		{
			prim.CreatePrimitive(p_mesh, PrimitiveTypes::P_PLANE);
		}


		ImGui::End();
	}
	
	return ret;
}
