#include "Panel.h"

Panel::Panel(std::string name) :name(name)
{

}

Panel::~Panel()
{

}

bool Panel::IsActive() const
{
	return is_active;
}

void Panel::ChangeActive()
{
	is_active = !is_active;
}