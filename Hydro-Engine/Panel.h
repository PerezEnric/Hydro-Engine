#pragma once

#include <string>

class Panel
{
public:
	Panel(std::string name);
	virtual ~Panel();

	virtual bool Update() { return true; };
	bool IsActive() const;
	void ChangeActive();

public:
	bool is_active = false;
	std::string name;

};
