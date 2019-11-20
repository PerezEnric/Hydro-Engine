#include "Component_Camera.h"
#include "GameObject.h"
#include "ImGui/imgui.h"

Component_Camera::Component_Camera(GameObject* gameObject, COMPONENT_TYPE type)
{
	comp_type_str = "camera";
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	//Common distance between near and far planes
	frustum.nearPlaneDistance = 0.1f; 
	frustum.farPlaneDistance = 100.0f;
	angle_fov = 90.0f;
	frustum.verticalFov = angle_fov * RADTODEG; //We have to depen on a certain FOV. Normally is the vertical FoV. We assign 90 degrees because is normally used in PC games (Wikipedia rules)
	frustum.horizontalFov = 2 * atanf(tan(frustum.verticalFov * 0.5) * 1.78f); //1.78 is the aspect ratio for 16:9 => 1920x1080p

	GO = gameObject;
	gameObject->cam = this;
	this->type = type;

	GO->b_camera = true;
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

nlohmann::json Component_Camera::SaveComponent()
{
	nlohmann::json ret;

	std::vector<float> float_helper;


	float_helper.push_back(l_scale.x);
	float_helper.push_back(l_scale.y);
	float_helper.push_back(l_scale.z);

	ret["l_scale"] = float_helper;
	float_helper.clear();

	float_helper.push_back(l_rotation.x);
	float_helper.push_back(l_rotation.y);
	float_helper.push_back(l_rotation.z);
	float_helper.push_back(l_rotation.w);

	ret["l_rotation"] = float_helper;

	float_helper.clear();

	float_helper.push_back(future_rotation.x);
	float_helper.push_back(future_rotation.y);
	float_helper.push_back(future_rotation.z);


	ret["future_rotation"] = float_helper;

	float_helper.clear();

	ret["angle fov"] = angle_fov;

	//frustum so i dont know if this is optimal or not.

	float_helper.push_back(frustum.front.x);
	float_helper.push_back(frustum.front.y);
	float_helper.push_back(frustum.front.z);

	ret["Frustrum front"] = float_helper;

	float_helper.clear();

	float_helper.push_back(frustum.up.x);
	float_helper.push_back(frustum.up.y);
	float_helper.push_back(frustum.up.z);

	ret["Frustrum up"] = float_helper;

	float_helper.clear();


	// frustrum distances

	ret["frustrum near plane distance"] = frustum.nearPlaneDistance;

	ret["frustrum far plane distance"] = frustum.farPlaneDistance;


	// frustrum position

	float_helper.push_back(frustum.pos.x);
	float_helper.push_back(frustum.pos.y);
	float_helper.push_back(frustum.pos.z);

	ret["Frustrum pos"] = float_helper;

	float_helper.clear();

	// frustrum Fov.

	ret["Frustrum vertical fov"] = frustum.verticalFov;
	ret["Frustrum horizontal fov"] = frustum.horizontalFov;

	

	if (GO != nullptr)
	{
		char* uuid_str = new char[80];
		sprintf(uuid_str, "%d", GO->my_uuid);

		ret["My parent UUID"] = uuid_str;
	}
	

	return ret;
}

void Component_Camera::LoadComponent(nlohmann::json & to_load)
{

	std::vector<float> scl = to_load["l_scale"].get<std::vector<float>>();

	std::vector<float> rot = to_load["l_rotation"].get<std::vector<float>>();

	std::vector<float> f_rot = to_load["future_rotation"].get<std::vector<float>>();


	l_scale.x = scl[0];
	l_scale.y = scl[1];
	l_scale.z = scl[2];

	l_rotation.x = rot[0];
	l_rotation.y = rot[1];
	l_rotation.z = rot[2];
	l_rotation.w = rot[3];

	future_rotation.x = f_rot[0];
	future_rotation.y = f_rot[1];
	future_rotation.z = f_rot[2];

	angle_fov = to_load["angle fov"].get<float>();

	//frustrum pure

	frustum.verticalFov = to_load["Frustrum vertical fov"].get<float>();
	frustum.horizontalFov = to_load["Frustrum horizontal fov"].get<float>();
	frustum.farPlaneDistance = to_load["frustrum far plane distance"].get<float>();
	frustum.nearPlaneDistance = to_load["frustrum near plane distance"].get<float>();

	std::vector<float> frs_f = to_load["Frustrum front"].get<std::vector<float>>();

	std::vector<float> frs_pos = to_load["Frustrum pos"].get<std::vector<float>>();

	std::vector<float> frs_up = to_load["Frustrum up"].get<std::vector<float>>();

	frustum.front.x = frs_f[0];
	frustum.front.y = frs_f[1];
	frustum.front.z = frs_f[2];

	frustum.pos.x = frs_pos[0];
	frustum.pos.y = frs_pos[1];
	frustum.pos.z = frs_pos[2];

	frustum.up.x = frs_up[0];
	frustum.up.y = frs_up[1];
	frustum.up.z = frs_up[2];



}
