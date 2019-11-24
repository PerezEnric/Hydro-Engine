#pragma once

#include "Panel.h"

class PanelGame : public Panel
{
public:
	PanelGame();
	~PanelGame();


	bool Update() override;

public:
	//bool is_active = true;
	bool show_panelgame = false;
};
