#include "Component_Camera.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

Component_Camera::Component_Camera(GameObject* gameObject, COMPONENT_TYPE type)
{
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	//Common distance between near and far planes
	frustum.nearPlaneDistance = 0.1f; 
	frustum.farPlaneDistance = 100.0f;

	frustum.verticalFov = 90.0f * RADTODEG; //We have to depen on a certain FOV. Normally is the vertical FoV. We assign 90 degrees because is normally used in PC games (Wikipedia rules)
	frustum.horizontalFov = 2 * atanf(tan(frustum.verticalFov * 0.5) * 1.78f); //1.78 is the aspect ratio for 16:9 => 1920x1080p

	gameObject->cam = this;
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
	glLineWidth(5.0f);
	glColor4f(1.f, 1.f, 0.f, 1.f);

	for (int i = 0; i < 8; ++i) {
		glVertex3f(frustum.CornerPoint(i).x, frustum.CornerPoint(i).y, frustum.CornerPoint(i).z);
	}

	glEnd();
}

void Component_Camera::ShowInfo()
{
	if (ImGui::Checkbox("DrawFrustum", &show_frustum))
	{

	}
}
