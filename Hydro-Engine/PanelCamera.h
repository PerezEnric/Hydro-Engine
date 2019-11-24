#pragma once

#include "Panel.h"

class PanelCamera: public Panel
{
public:
	PanelCamera();
	virtual ~PanelCamera();

	bool Update() override;

private:

public:
	bool show_panelcam = true;
};
