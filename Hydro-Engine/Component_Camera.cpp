#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Component_Camera.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

Component_Camera::Component_Camera(GameObject* gameObject, COMPONENT_TYPE type)
{
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3(0.0f, 0.0f, -10.0f);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	//Common distance between near and far planes
	frustum.nearPlaneDistance = 0.1f; 
	frustum.farPlaneDistance = 100.0f;
	angle_fov = 90.0f;
	frustum.verticalFov = angle_fov * RADTODEG; //We have to depen on a certain FOV. Normally is the vertical FoV. We assign 90 degrees because is normally used in PC games (Wikipedia rules)
	frustum.horizontalFov = 2 * atanf(tan(frustum.verticalFov * 0.5) * 1.78f); //1.78 is the aspect ratio for 16:9 => 1920x1080p

	gameObject->cam = this;
	this->type = type;
}

Component_Camera::Component_Camera()
{
}

Component_Camera::~Component_Camera()
{
}

bool Component_Camera::Update()
{
	if (show_frustum)
		DrawFrustrum();

	return true;
}

void Component_Camera::FrustrumLook(const float3& position)
{
	float3 camera_direction = position - frustum.pos;

	float3x3 trans_matrix = float3x3::LookAt(frustum.front, camera_direction.Normalized(), frustum.up, float3::unitY);

	frustum.front = trans_matrix.MulDir(frustum.front).Normalized();
	frustum.up = trans_matrix.MulDir(frustum.up).Normalized();
}

void Component_Camera::DrawFrustrum()
{
	glBegin(GL_LINES);
	glLineWidth(30.0f);

	// Diagonals
	for (int i = 0; i < 8; ++i) {
		glVertex3f(frustum.CornerPoint(i).x, frustum.CornerPoint(i).y, frustum.CornerPoint(i).z);
	}

	// Near Plane
	glVertex3f(frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z);
	glVertex3f(frustum.CornerPoint(2).x, frustum.CornerPoint(2).y, frustum.CornerPoint(2).z);

	glVertex3f(frustum.CornerPoint(2).x, frustum.CornerPoint(2).y, frustum.CornerPoint(2).z);
	glVertex3f(frustum.CornerPoint(6).x, frustum.CornerPoint(6).y, frustum.CornerPoint(6).z);

	glVertex3f(frustum.CornerPoint(6).x, frustum.CornerPoint(6).y, frustum.CornerPoint(6).z);
	glVertex3f(frustum.CornerPoint(4).x, frustum.CornerPoint(4).y, frustum.CornerPoint(4).z);

	glVertex3f(frustum.CornerPoint(4).x, frustum.CornerPoint(4).y, frustum.CornerPoint(4).z);
	glVertex3f(frustum.CornerPoint(0).x, frustum.CornerPoint(0).y, frustum.CornerPoint(0).z);

	// Far Plane
	glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);
	glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);

	glVertex3f(frustum.CornerPoint(3).x, frustum.CornerPoint(3).y, frustum.CornerPoint(3).z);
	glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);

	glVertex3f(frustum.CornerPoint(7).x, frustum.CornerPoint(7).y, frustum.CornerPoint(7).z);
	glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);

	glVertex3f(frustum.CornerPoint(5).x, frustum.CornerPoint(5).y, frustum.CornerPoint(5).z);
	glVertex3f(frustum.CornerPoint(1).x, frustum.CornerPoint(1).y, frustum.CornerPoint(1).z);

	glEnd();
}

void Component_Camera::ShowInfo()
{
	if (ImGui::Checkbox("Draw Frustum", &show_frustum))
	{
	}

	ImGui::Separator();

	if (ImGui::DragFloat("Near Plane", &frustum.nearPlaneDistance, 0.1f, 0.0f, frustum.farPlaneDistance))
	{
		ChangeNearPlaneDist(frustum.nearPlaneDistance);
	}

	if (ImGui::DragFloat("Far Plane", &frustum.farPlaneDistance, 0.1f, frustum.nearPlaneDistance, 1000.0f))
	{
		ChangeFarPlaneDist(frustum.farPlaneDistance);
	}

	ImGui::Separator();

	if (ImGui::DragFloat3("Position", &frustum.pos[3], 0.1f))
	{
		SetFrustumPosition(frustum.pos);
	}

	if (ImGui::DragFloat3("Rotation", &future_rotation[3], 0.1f))
	{
		SetFrustumRotation(future_rotation);
	}

	if (ImGui::DragFloat("FOV", &angle_fov, 0.1f))
	{
		ChangeFOV(angle_fov);
	}
}

void Component_Camera::ChangeNearPlaneDist(float distance)
{
	if (distance > 0.0f && distance < frustum.farPlaneDistance)
		frustum.nearPlaneDistance = distance;

}

void Component_Camera::ChangeFarPlaneDist(float distance)
{
	if (distance > frustum.nearPlaneDistance)
		frustum.farPlaneDistance = distance;
}

void Component_Camera::ChangeFOV(float angle)
{
	frustum.verticalFov = angle * DEGTORAD;
	frustum.horizontalFov = 2 * atanf(tan(frustum.verticalFov * 0.5) * 1.78f);
}

void Component_Camera::SetFrustumPosition(float3 position)
{
	frustum.pos = position;
	SetFrustumTransform();
}

void Component_Camera::SetFrustumTransform()
{
	future_rotation = l_rotation.ToEulerXYZ(); // we set the quaternion into a float3 so we can use it in SetRotation()
	future_rotation *= RADTODEG; // the previous function returns the rotation in radians so we put in degrees
	my_current_matrix = float4x4::FromTRS(frustum.pos, l_rotation, l_scale);
}

void Component_Camera::SetFrustumRotation(float3 rot)
{
	l_rotation = Quat::FromEulerXYZ(rot.x * DEGTORAD, rot.y * DEGTORAD, rot.z * DEGTORAD);
	SetFrustumTransform();
}

int Component_Camera::ContainsAABBox(const AABB& refbox) const
{
	float3 vCorner[8];
	int iTotalIn = 0;
	refbox.GetCornerPoints(vCorner);

	Plane m_plane[6];
	frustum.GetPlanes(m_plane); //we need to match the m_plane array with frustum planes 

	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			if (!m_plane[p].IsOnPositiveSide(vCorner[i])) {
				iPtIn = 0;
				--iInCount;
			}
		}
		if (iInCount == 0)
		{
			LOG("IS OUTSIDEEEE");
			return(OUTSIDE);
		}
		iTotalIn += iPtIn;
	}
	if (iTotalIn == 6)
	{
		LOG("IS INSIDE!!!!!!");
		return(INSIDE);
	}

	else
	{
		LOG("THIS SHIT IS SO STRANGE")
		return(INTERSECT);
	}

	//float3 b_corners[8];
	//refbox.GetCornerPoints(b_corners);

	//for (uint plane_sides = 0; plane_sides < 6; ++plane_sides)
	//{
	//	int inside_corners = 8;
	//	for (uint point = 0; point < 8; ++point)
	//	{
	//		if (frustum.GetPlane(plane_sides).IsOnPositiveSide(b_corners[point]))
	//			--inside_corners;
	//	}
	//	// We dont need to know (at the moment) the exact number of corners that are in
	//	if (inside_corners == 0) //We just look if some corner is inside to cull or not
	//		return OUTSIDE;
	//}
	//return INSIDE;
}

float4x4 Component_Camera::GetViewMatrix() const
{
	float4x4 view_matrix = frustum.ViewMatrix();
	return view_matrix.Transposed();
}

float4x4 Component_Camera::GetProjectionMatrix() const
{
	float4x4 projection_matrix = frustum.ProjectionMatrix();
	return projection_matrix.Transposed();
}
