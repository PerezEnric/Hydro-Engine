#pragma once

#include "Panel.h"

class PanelSaveAndLoad : public Panel
{
public:
	PanelSaveAndLoad();
	~PanelSaveAndLoad();

	bool Update() override;
	void SaveScene();
	void LoadScene(std::string path);
	char str1[128] = "";
public:
	bool saving = false;
};