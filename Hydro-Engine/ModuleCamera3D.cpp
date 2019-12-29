#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "Component_Mesh.h"
#include "GameObject.h"
#include "MathGeoLib/include/Math/Quat.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	/*CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);*/

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	main_cam = new Component_Camera(nullptr, COMPONENT_TYPE::CAMERA, true);
	//Test = new Component_Camera(nullptr, CAMERA, true);
	main_cam->FrustrumLook(float3::zero);
	Position = main_cam->frustum.pos;
	//Test->FrustrumLook(float3::zero);
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	delete main_cam;
	main_cam = nullptr;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	float3 newPos(0, 0, 0);
	float speed = 3.0f * dt;
	float wheelSpeed = 6.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		CentreGOView();

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{	
		newPos += main_cam->frustum.front * speed;
	}
	

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		newPos -= main_cam->frustum.front * speed;
	}
		


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		newPos -= main_cam->frustum.WorldRight() * speed;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		newPos += main_cam->frustum.WorldRight() * speed;
	}

	if (!newPos.Equals(float3::zero)) //if it is zero at starting the scene it crashes
	{
		main_cam->frustum.Translate(newPos);
		Position += newPos;
		Reference += newPos;
	}
		

	// Mouse motion ----------------

	if(App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();
		float3 rot = main_cam->frustum.pos;
		float Sensitivity = 0.25f * dt;

		//Position -= Reference;

		if (dx)
		{
			Quat rot_quat = Quat::RotateY(dx * Sensitivity);

			main_cam->frustum.front = rot_quat.Mul(main_cam->frustum.front).Normalized();
			main_cam->frustum.up = rot_quat.Mul(main_cam->frustum.up).Normalized();
		}

		if (dy)
		{
			Quat rot_quat = Quat::RotateAxisAngle(main_cam->frustum.WorldRight(), dy * Sensitivity);

			float3 this_up = rot_quat.Mul(main_cam->frustum.up).Normalized();

			if (this_up.y > 0.0f)
			{
				main_cam->frustum.up = this_up;
				main_cam->frustum.front = rot_quat.Mul(main_cam->frustum.front).Normalized();
			}

		}

	}

	//Zoom

	if (App->input->GetMouseZ() < 0)
	{
		newPos -= main_cam->frustum.front * wheelSpeed;
		main_cam->frustum.Translate(newPos);
		Position += newPos;
		Reference += newPos;
	}

	if (App->input->GetMouseZ() > 0)
	{
		newPos += main_cam->frustum.front * wheelSpeed;

		main_cam->frustum.Translate(newPos);
		Position += newPos;
		Reference += newPos;
	}

	//Mouse picking

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		CastRay();
	}

	/*if (draw_ray)
		DrawRay();
*/

	//// Recalculate matrix -------------
	//CalculateViewMatrix();


/*
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		Test->frustum.pos.z += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		Test->frustum.pos.x += 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		Test->frustum.pos.z -= 1;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		Test->frustum.pos.x -= 1;
	}

	Test->DrawFrustrum();

	main_cam->show_frustum = true;

	main_cam->Update();*/

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
//void ModuleCamera3D::Look(const float3 &Position, const float3&Reference, bool RotateAroundReference)
//{
//	this->Position = Position;
//	this->Reference = Reference;
//
//	Z = normalize(Position - Reference);
//	X = normalize(cross(float3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//
//	if(!RotateAroundReference)
//	{
//		this->Reference = this->Position;
//		this->Position += Z * 0.05f;
//	}
//
//	CalculateViewMatrix();
//}

// -----------------------------------------------------------------
//void ModuleCamera3D::LookAt( const float3&Spot)
//{
//	Reference = Spot;
//
//	Z = normalize(Position - Reference);
//	X = normalize(cross(float3(0.0f, 1.0f, 0.0f), Z));
//	Y = cross(Z, X);
//
//	CalculateViewMatrix();
//}


// -----------------------------------------------------------------
//void ModuleCamera3D::Move(const float3&Movement)
//{
//	Position += Movement;
//	Reference += Movement;
//
//	CalculateViewMatrix();
//}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::CentreGOView()
{
	if (App->scene_intro->selected != nullptr)
	{
		OBB bbox = App->scene_intro->selected->CreateOBB(); //we return the bounding box created with the component mesh
		float3 new_reference = bbox.CenterPoint(); //we define a new reference to look at. It is the center point of the bbox
		float3 new_position = bbox.CenterPoint() * 5; //we define a new position were the camera will place

		Position.x = new_position.x;
		Position.y = new_position.y;
		Position.z = new_position.z;

		Reference.x = new_reference.x;
		Reference.y = new_reference.y;
		Reference.z = new_reference.z; //we assign all the new values to previous position and reference

		main_cam->FrustrumLook(Reference); //Finally look at the reference with all the new values
	}
}

void ModuleCamera3D::CastRay()
{
	float mouseNorm_x = -1.0f + 2.0f * App->input->GetMouseX() / App->window->width;
	float mouseNorm_y = 1.0f - 2.0f * App->input->GetMouseY() / App->window->height;

	picking = LineSegment(main_cam->frustum.UnProjectLineSegment(mouseNorm_x, mouseNorm_y));

	App->scene_intro->RayTestAABB(picking);
	draw_ray = true;
}

void ModuleCamera3D::DrawRay()
{
	glColor3f(200, 255, 0.0f);
	glLineWidth(0.2);
	GLfloat pointA[3] = { picking.a.x, picking.a.y, picking.a.z };
	GLfloat pointB[3] = { picking.b.x, picking.b.y, picking.b.z };

	glBegin(GL_LINES);
	glVertex3fv(pointA);
	glVertex3fv(pointB);
	glEnd();
}

// -----------------------------------------------------------------
//void ModuleCamera3D::CalculateViewMatrix()
//{
//	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
//	ViewMatrixInverse = inverse(ViewMatrix);
//}
