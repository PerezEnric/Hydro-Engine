#include "Resource.h"

Resource::Resource(uint uuid, RESOURCE_TYPE type)
{
}

Resource::~Resource()
{
}

RESOURCE_TYPE Resource::GetType()
{
	return RESOURCE_TYPE();
}

uint Resource::GetUUID()
{
	return uint();
}

std::string Resource::GetOriginalPath()
{
	return std::string();
}

std::string Resource::GetOwnPath()
{
	return std::string();
}

bool Resource::IsLoadedInMemory()
{
	return false;
}

virtual bool Resource::LoadToMemory()
{
	return false;
}

uint Resource::CountReferences()
{
	return uint();
}
