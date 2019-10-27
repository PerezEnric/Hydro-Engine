#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void EnableRenderSettings(int render_set, bool is_active);
	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool gl_depth_test = false;
	bool gl_cull_face = false;
	bool gl_lighting = true;
	bool gl_color_material = true;
	bool gl_texture_2D = false;
	bool gl_blend = false;
	bool is_wireframe = false;
};