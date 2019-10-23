#pragma once

#include "Panel.h"

class PanelHierarchy : public Panel
{
public:
	PanelHierarchy();
	~PanelHierarchy();

	bool Update() override;
	void CreateNode();

public:
	bool show_hierarchy = true;
};