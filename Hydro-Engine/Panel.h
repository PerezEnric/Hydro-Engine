#pragma once

#include <string>

class Panel
{
public:
	Panel();
	virtual ~Panel();

	virtual bool Update() { return true; };
	bool IsActive() const;
	void ChangeActive();

public:
	bool is_active = false;
	std::string name;

};
