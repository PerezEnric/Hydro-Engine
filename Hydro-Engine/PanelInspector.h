#pragma once

#include "Panel.h"

class PanelInspector :public Panel
{
public:
	PanelInspector();
	~PanelInspector();

	bool Update() override;

public:
	bool show_inspector = true;
};