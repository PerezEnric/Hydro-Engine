#pragma once

#include "Panel.h"

class Component_Mesh;
class Component_Transform;
class Component_Texture;

class PanelInspector :public Panel
{
public:
	PanelInspector();
	~PanelInspector();

	void TransformWindow();
	void MeshWindow();
	void TextureWindow();
	void FrustumWindow();

	bool Update() override;

public:
	bool show_inspector = true;
	bool show_bbox = false;
};